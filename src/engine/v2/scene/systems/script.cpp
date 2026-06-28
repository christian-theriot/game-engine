#include <engine/v2/scene/systems/script.hpp>
#include <engine/v2/scene/components/script.hpp>
#include <engine/v2/scene/world.hpp>
#include <iostream>
#include <fstream>

Engine::Scene::Systems::LuaScript::LuaScript()
{
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table);

    lua.set_function("log", [](const std::string &message)
                     { std::cout << "[Lua] " << message << std::endl; });
}
Engine::Scene::Systems::LuaScript::~LuaScript()
{
    for (auto &entity : world->getEntitiesWithComponent<Engine::Scene::Components::LuaScript>())
    {
        auto *scriptComponent = entity->getComponent<Engine::Scene::Components::LuaScript>();
        if (scriptComponent && scriptComponent->loaded)
        {
            scriptComponent->instance = sol::table();
            scriptComponent->loaded = false;
        }
    }

    lua.collect_garbage();
}
bool Engine::Scene::Systems::LuaScript::load(Entity *entity)
{
    try
    {
        auto *scriptComponent = entity->getComponent<Engine::Scene::Components::LuaScript>();
        sol::load_result chunk = lua.load_file(scriptComponent->scriptPath);
        if (!chunk.valid())
        {
            sol::error err = chunk;
            std::cerr << "Error loading Lua script: " << err.what() << std::endl;
            return false;
        }

        sol::protected_function_result result = chunk();
        if (!result.valid())
        {
            sol::error err = result;
            std::cerr << "Error executing Lua script: " << err.what() << std::endl;
            return false;
        }

        if (result.get_type() == sol::type::table)
        {
            scriptComponent->instance = result;
            scriptComponent->loaded = true;
        }
        else
        {
            scriptComponent->instance = lua.create_table();
            scriptComponent->loaded = false;
        }
        return true;
    }
    catch (const sol::error &e)
    {
        std::cerr << "Error loading Lua script: " << e.what() << std::endl;
        return false;
    }
}
void Engine::Scene::Systems::LuaScript::update(World *world, float deltaTime)
{
    for (auto &entity : world->getEntities())
    {
        auto *scriptComponent = entity->getComponent<Engine::Scene::Components::LuaScript>();
        if (scriptComponent && scriptComponent->loaded)
        {
            sol::protected_function updateFunc = scriptComponent->instance["onUpdate"];
            if (updateFunc.valid())
            {
                sol::protected_function_result result = updateFunc(scriptComponent->instance, deltaTime);
                if (!result.valid())
                {
                    sol::error err = result;
                    std::cerr << "Error executing Lua update function: " << err.what() << std::endl;
                }
            }
        }
    }
}

std::vector<uint8_t> wasmReadFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cerr << "Failed to open WASM file: " << path << std::endl;
        return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        std::cerr << "Failed to read WASM file: " << path << std::endl;
        return {};
    }

    return buffer;
}
void wasmPrintError(wasmtime_error_t *error)
{
    if (!error)
        return;
    wasm_name_t msg;
    wasmtime_error_message(error, &msg);
    std::cerr << "WASM Error: " << std::string(msg.data, msg.size) << std::endl;
    wasm_byte_vec_delete(&msg);
    wasmtime_error_delete(error);
}
static void wasmPrintTrap(wasm_trap_t *trap)
{
    if (!trap)
        return;
    wasm_name_t msg;
    wasm_trap_message(trap, &msg);
    std::cerr << "WASM Trap: " << std::string(msg.data, msg.size) << std::endl;
    wasm_byte_vec_delete(&msg);
    wasm_trap_delete(trap);
}

Engine::Scene::Systems::WasmScript::~WasmScript()
{
    for (auto &entity : world->getEntitiesWithComponent<Engine::Scene::Components::WasmScript>())
    {
        unload(entity);
    }
}
void Engine::Scene::Systems::WasmScript::unload(Entity *entity)
{
    auto *scriptComponent = entity->getComponent<Engine::Scene::Components::WasmScript>();
    if (scriptComponent && scriptComponent->loaded)
    {
        if (scriptComponent->module)
        {
            wasmtime_module_delete(scriptComponent->module);
            scriptComponent->module = nullptr;
        }
        if (scriptComponent->store)
        {
            wasmtime_store_delete(scriptComponent->store);
            scriptComponent->store = nullptr;
        }
        if (scriptComponent->engine)
        {
            wasm_engine_delete(scriptComponent->engine);
            scriptComponent->engine = nullptr;
        }
        scriptComponent->loaded = false;
    }
}
bool Engine::Scene::Systems::WasmScript::load(Entity *entity)
{
    auto *scriptComponent = entity->getComponent<Engine::Scene::Components::WasmScript>();
    if (!scriptComponent)
        return false;

    auto wasmBytes = wasmReadFile(scriptComponent->scriptPath);
    if (wasmBytes.empty())
        return false;

    unload(entity);

    scriptComponent->engine = wasm_engine_new();
    if (!scriptComponent->engine)
    {
        std::cerr << "Failed to create WASM engine." << std::endl;
        return false;
    }

    scriptComponent->store = wasmtime_store_new(scriptComponent->engine, nullptr, nullptr);
    if (!scriptComponent->store)
    {
        std::cerr << "Failed to create WASM store." << std::endl;
        return false;
    }

    wasmtime_error_t *error = wasmtime_module_new(scriptComponent->engine, wasmBytes.data(), wasmBytes.size(), &scriptComponent->module);
    if (error)
    {
        wasmPrintError(error);
        return false;
    }

    wasmtime_context_t *context = wasmtime_store_context(scriptComponent->store);
    wasm_trap_t *trap = nullptr;

    auto makeFunc = [&](std::vector<wasm_valtype_t *> params, std::vector<wasm_valtype_t *> results, wasmtime_func_callback_t callback) -> wasmtime_extern_t
    {
        wasm_valtype_vec_t p, r;
        wasm_valtype_vec_new(&p, params.size(), params.data());
        if (results.empty())
        {
            wasm_valtype_vec_new_empty(&r);
        }
        else
        {
            wasm_valtype_vec_new(&r, results.size(), results.data());
        }

        wasm_functype_t *funcType = wasm_functype_new(&p, &r);
        wasmtime_func_t func;
        wasmtime_func_new(context, funcType, callback, world, nullptr, &func);
        wasm_functype_delete(funcType);

        wasmtime_extern_t externFunc;
        externFunc.kind = WASMTIME_EXTERN_FUNC;
        externFunc.of.func = func;
        return externFunc;
    };
    std::vector<wasmtime_extern_t> wasmExports;
    std::unordered_map<std::string, wasmtime_extern_t> exports;
    exports["abort"] = makeFunc({wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32)}, {}, [](void *env, wasmtime_caller_t *caller, const wasmtime_val_t *args, size_t nargs, wasmtime_val_t *results, size_t nresults) -> wasm_trap_t *
                                {
        
        std::cerr << "[Wasm] abort() called externally" << std::endl;
        return nullptr; });
    exports["log"] = makeFunc({wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32)}, {}, [](void *env, wasmtime_caller_t *caller, const wasmtime_val_t *args, size_t nargs, wasmtime_val_t *results, size_t nresults) -> wasm_trap_t *
                              {
        
        if (nargs != 2)
            return nullptr;

        int32_t ptr = args[0].of.i32;
        int32_t len = args[1].of.i32;

        // Get the guest's exported memory named "memory"
        wasmtime_extern_t memoryExport;
        if (!wasmtime_caller_export_get(caller, "memory", 6, &memoryExport) || memoryExport.kind != WASMTIME_EXTERN_MEMORY)
        {
            std::cerr << "[Wasm] Failed to access guest memory for logging" << std::endl;
            return nullptr;
        }

        wasmtime_context_t *context = wasmtime_caller_context(caller);
        uint8_t *memoryData = wasmtime_memory_data(context, &memoryExport.of.memory);
        size_t memorySize = wasmtime_memory_data_size(context, &memoryExport.of.memory);

        if (ptr < 0 || len < 0 || static_cast<size_t>(ptr + len) > memorySize)
        {
            std::cerr << "[Wasm] Invalid memory access for logging" << std::endl;
            return nullptr;
        }

        std::string message(reinterpret_cast<char *>(memoryData + ptr), len);
        std::cout << "[Wasm] " << message << std::endl;

        return nullptr; });

    wasmExports.emplace_back(exports["abort"]);
    wasmExports.emplace_back(exports["log"]);

    error = wasmtime_instance_new(context, scriptComponent->module, wasmExports.data(), wasmExports.size(), &scriptComponent->instance, &trap);
    if (error || trap)
    {
        wasmPrintError(error);
        wasmPrintTrap(trap);
        return false;
    }

    wasmtime_extern_t ext;
    bool found = wasmtime_instance_export_get(context, &scriptComponent->instance, "onStart", 7, &ext);
    if (found && ext.kind == WASMTIME_EXTERN_FUNC)
    {
        scriptComponent->onStart = ext.of.func;
        scriptComponent->hasOnStart = true;
    }
    else
    {
        scriptComponent->hasOnStart = false;
    }

    found = wasmtime_instance_export_get(context, &scriptComponent->instance, "onUpdate", 8, &ext);
    if (found && ext.kind == WASMTIME_EXTERN_FUNC)
    {
        scriptComponent->onUpdate = ext.of.func;
        scriptComponent->hasOnUpdate = true;
    }
    else
    {
        scriptComponent->hasOnUpdate = false;
    }

    scriptComponent->loaded = true;
    scriptComponent->hasInstance = true;

    if (scriptComponent->hasOnStart)
    {
        wasm_trap_t *startTrap = nullptr;
        wasmtime_error_t *startError = wasmtime_func_call(context, &scriptComponent->onStart, nullptr, 0, nullptr, 0, &startTrap);
        if (startError || startTrap)
        {
            wasmPrintError(startError);
            wasmPrintTrap(startTrap);
            return false;
        }
    }

    std::cout << "[Wasm] Loaded script: " << scriptComponent->scriptPath << std::endl;

    return true;
}
void Engine::Scene::Systems::WasmScript::update(World *world, float deltaTime)
{
    for (auto &entity : world->getEntitiesWithComponent<Engine::Scene::Components::WasmScript>())
    {
        auto *scriptComponent = entity->getComponent<Engine::Scene::Components::WasmScript>();
        if (scriptComponent && scriptComponent->loaded && scriptComponent->hasOnUpdate)
        {
            wasmtime_context_t *context = wasmtime_store_context(scriptComponent->store);
            wasmtime_val_t params[1];
            params[0].kind = WASM_F32;
            params[0].of.f32 = deltaTime;

            wasm_trap_t *trap = nullptr;
            wasmtime_error_t *error = wasmtime_func_call(context, &scriptComponent->onUpdate, params, 1, nullptr, 0, &trap);

            if (error || trap)
            {
                wasmPrintError(error);
                wasmPrintTrap(trap);
            }
        }
    }
}