#include <engine/bindings.hpp>
#include <engine/components/transform.hpp>
#include <engine/transform.hpp>
#include <engine/entity.hpp>
#include <engine/world.hpp>

void Engine::LuaBindings::bindAll(sol::state &lua, World *registry)
{
    // Logging
    lua.set_function("log", [](const std::string &message)
                     { std::cout << "[Lua] " << message << std::endl; });

    // glm::vec3
    lua.new_usertype<glm::vec3>("vec3",
                                sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
                                "x", &glm::vec3::x,
                                "y", &glm::vec3::y,
                                "z", &glm::vec3::z);
}

wasm_trap_t *hostAbort(void *env, wasmtime_caller_t *caller, const wasmtime_val_t *args, size_t nargs, wasmtime_val_t *results, size_t nresults)
{
    std::cerr << "[Wasm] abort() called externally" << std::endl;
    return nullptr;
}

wasm_trap_t *hostLog(void *env, wasmtime_caller_t *caller, const wasmtime_val_t *args, size_t nargs, wasmtime_val_t *results, size_t nresults)
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

    return nullptr;
}

void Engine::WasmBindings::bindAll(std::vector<wasmtime_extern_t> &wasmExports, Engine::WasmScriptingEngine *self)
{
    // Import 0: env.abort
    wasm_valtype_t *ptr0[4] = {
        wasm_valtype_new(WASM_I32), // message pointer
        wasm_valtype_new(WASM_I32), // message length
        wasm_valtype_new(WASM_I32), // file name pointer
        wasm_valtype_new(WASM_I32)  // file name length
    };
    wasm_valtype_vec_t params0, resultsTypes0;
    wasm_valtype_vec_new(&params0, 4, ptr0);
    wasm_valtype_vec_new_empty(&resultsTypes0);
    wasm_functype_t *abortType = wasm_functype_new(&params0, &resultsTypes0);

    wasmtime_func_t abortFunc;
    auto context = self->getContext();
    auto world = self->getWorld();
    wasmtime_func_new(context, abortType, hostAbort, world, nullptr, &abortFunc);
    wasm_functype_delete(abortType);

    wasmtime_extern_t abortExtern;
    abortExtern.kind = WASMTIME_EXTERN_FUNC;
    abortExtern.of.func = abortFunc;
    wasmExports.push_back(abortExtern);

    // Import 1: env.log (i32, i32) -> ()
    wasm_valtype_t *ptr1[2] = {wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32)};
    wasm_valtype_vec_t params1, resultsTypes1;
    wasm_valtype_vec_new(&params1, 2, ptr1);
    wasm_valtype_vec_new_empty(&resultsTypes1);

    wasm_functype_t *logType = wasm_functype_new(&params1, &resultsTypes1);

    wasmtime_func_t logFunc;
    wasmtime_func_new(context, logType, hostLog, world, nullptr, &logFunc);
    wasm_functype_delete(logType);

    wasmtime_extern_t logExtern;
    logExtern.kind = WASMTIME_EXTERN_FUNC;
    logExtern.of.func = logFunc;
    wasmExports.push_back(logExtern);
}
