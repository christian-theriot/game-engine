/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/scripting.hpp>
#include <engine/bindings.hpp>
#include <engine/world.hpp>
#include <iostream>
#include <fstream>

Engine::LuaScriptingEngine::LuaScriptingEngine()
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
}
bool Engine::LuaScriptingEngine::load(const std::string &scriptPath)
{
    sol::protected_function_result result = lua.safe_script_file(scriptPath, sol::script_pass_on_error);

    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[Lua] Failed to load '" << scriptPath << "': " << err.what() << std::endl;
        return false;
    }
    return true;
}
bool Engine::LuaScriptingEngine::run(const std::string &code)
{
    sol::protected_function_result result = lua.safe_script(code, sol::script_pass_on_error);

    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[Lua] Runtime error: " << err.what() << std::endl;
        return false;
    }
    return true;
}
sol::state &Engine::LuaScriptingEngine::state()
{
    return lua;
}

bool readFile(const std::string &path, std::vector<uint8_t> &out)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cerr << "[Wasm] Cannot open '" << path << "'" << std::endl;
        return false;
    }
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    out.resize(size);
    file.read(reinterpret_cast<char *>(out.data()), size);
    return true;
}

void reportError(const std::string &what, wasmtime_error_t *error, wasm_trap_t *trap)
{
    wasm_byte_vec_t message;
    if (error)
    {
        wasmtime_error_message(error, &message);
        wasmtime_error_delete(error);
    }
    else if (trap)
    {
        wasm_trap_message(trap, &message);
        wasm_trap_delete(trap);
    }
    else
    {
        std::cerr << "[Wasm] " << what << ": unknown error" << std::endl;
        return;
    }

    std::cerr << "[Wasm] " << what << ": " << std::string(message.data, message.size) << std::endl;
    wasm_byte_vec_delete(&message);
}

Engine::WasmScriptingEngine::WasmScriptingEngine()
    : engine(wasm_engine_new()),
      store(wasmtime_store_new(engine, nullptr, nullptr)),
      context(wasmtime_store_context(store)),
      module(nullptr),
      instance({}),
      instantiated(false)
{
}
Engine::WasmScriptingEngine::~WasmScriptingEngine()
{
    if (module)
        wasmtime_module_delete(module);
    if (store)
        wasmtime_store_delete(store);
    if (engine)
        wasm_engine_delete(engine);
}
bool Engine::WasmScriptingEngine::load(const std::string &filePath)
{
    std::vector<uint8_t> wasmBytes;
    if (!readFile(filePath, wasmBytes))
        return false;

    wasmtime_error_t *error = nullptr;
    error = wasmtime_module_new(engine, wasmBytes.data(), wasmBytes.size(), &module);
    if (error)
    {
        reportError("Failed to compile module", error, nullptr);
        return false;
    }

    std::vector<wasmtime_extern_t> imports;
    Engine::WasmBindings().bindAll(imports, this);

    wasm_trap_t *trap = nullptr;
    error = wasmtime_instance_new(context, module, imports.data(), imports.size(), &instance, &trap);
    if (error || trap)
    {
        reportError("Failed to instantiate module", error, trap);
        return false;
    }

    instantiated = true;
    return true;
}
bool Engine::WasmScriptingEngine::call(const std::string &name)
{
    if (!instantiated)
    {
        std::cerr << "[Wasm] Cannot call function: module not instantiated" << std::endl;
        return false;
    }

    wasmtime_extern_t funcExtern;
    bool found = wasmtime_instance_export_get(context, &instance, name.c_str(), name.size(), &funcExtern);
    if (!found || funcExtern.kind != WASMTIME_EXTERN_FUNC)
    {
        std::cerr << "[Wasm] Exported function '" << name << "' not found" << std::endl;
        return false;
    }

    wasm_trap_t *trap = nullptr;
    wasmtime_error_t *error = wasmtime_func_call(context, &funcExtern.of.func, nullptr, 0, nullptr, 0, &trap);
    if (error || trap)
    {
        reportError("Failed to call '" + name + "'", error, trap);
        return false;
    }

    return true;
}
bool Engine::WasmScriptingEngine::call(const std::string &name, float arg)
{
    if (!instantiated)
    {
        std::cerr << "[Wasm] Cannot call function: module not instantiated" << std::endl;
        return false;
    }

    wasmtime_extern_t funcExtern;
    bool found = wasmtime_instance_export_get(context, &instance, name.c_str(), name.size(), &funcExtern);
    if (!found || funcExtern.kind != WASMTIME_EXTERN_FUNC)
    {
        std::cerr << "[Wasm] Exported function '" << name << "' not found" << std::endl;
        return false;
    }

    wasmtime_val_t args[1] = {wasmtime_val_t{.kind = WASMTIME_F32, .of.f32 = arg}};
    wasm_trap_t *trap = nullptr;
    wasmtime_error_t *error = wasmtime_func_call(context, &funcExtern.of.func, args, 1, nullptr, 0, &trap);
    if (error || trap)
    {
        reportError("Failed to call '" + name + "'", error, trap);
        return false;
    }

    return true;
}
bool Engine::WasmScriptingEngine::hasExport(const std::string &name) const
{
    if (!instantiated)
    {
        std::cerr << "[Wasm] Cannot check export: module not instantiated" << std::endl;
        return false;
    }

    wasmtime_extern_t funcExtern;
    bool found = wasmtime_instance_export_get(context, &instance, name.c_str(), name.size(), &funcExtern);
    return found && funcExtern.kind == WASMTIME_EXTERN_FUNC;
}