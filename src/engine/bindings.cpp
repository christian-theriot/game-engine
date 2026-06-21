#include <engine/bindings.hpp>
#include <engine/components/transform.hpp>
#include <engine/transform.hpp>
#include <engine/entity.hpp>
#include <engine/world.hpp>

void Engine::LuaBindings::bindAll(sol::state &lua, World *world)
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

    lua.new_usertype<Engine::Entity>("Entity", "findById", [world](uint64_t id) -> Entity *
                                     {
                                         auto* entity = world->getEntityById(id);
                                         if (!entity) {
                                            std::cerr << "[Lua] Entity with ID " << id << " not found" << std::endl;
                                            return nullptr;
                                         }
                                        return entity; }, "getId", [](Entity *entity)
                                     { return entity ? entity->getId() : 0; }, "setPosition", [world](Entity *entity, const glm::vec3 &pos)
                                     {
                                        if (entity && entity->hasComponent<Engine::Components::TransformComponent>()) {
                                            auto* transform = entity->getComponent<Engine::Components::TransformComponent>();
                                            transform->getTransform()->setPosition(pos);
                                        } else {
                                            std::cerr << "[Lua] Entity does not have a TransformComponent or is null" << std::endl;
                                        } });

    // setPosition(entityId, vec3)
    lua.set_function("setPosition", [world](int entityId, const glm::vec3 &position)
                     {
                         auto* entity = world->getEntityById(entityId);
                         if (entity && entity->hasComponent<Engine::Components::TransformComponent>())
                         {
                             auto *transform = entity->getComponent<Engine::Components::TransformComponent>();
                             transform->getTransform()->setPosition(position);
                         } });
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

wasm_trap_t *getEntityById(void *env, wasmtime_caller_t *caller, const wasmtime_val_t *args, size_t nargs, wasmtime_val_t *results, size_t nresults)
{
    if (nargs != 1 || nresults != 1)
        return nullptr;

    int32_t id = args[0].of.i32;
    Engine::World *world = static_cast<Engine::World *>(env);
    Engine::Entity *entity = world->getEntityById(id);
    if (!entity)
    {
        std::cerr << "[Wasm] Entity with ID " << id << " not found" << std::endl;
        results[0].kind = WASMTIME_I32;
        results[0].of.i32 = 0;
    }
    else
    {
        results[0].kind = WASMTIME_I32;
        results[0].of.i32 = static_cast<int32_t>(entity->getId());
    }
    return nullptr;
}

wasm_trap_t *setEntityPosition(void *env, wasmtime_caller_t *caller, const wasmtime_val_t *args, size_t nargs, wasmtime_val_t *results, size_t nresults)
{
    if (nargs != 4)
        return nullptr;

    int32_t id = args[0].of.i32;
    float x = args[1].of.f32;
    float y = args[2].of.f32;
    float z = args[3].of.f32;
    Engine::World *world = static_cast<Engine::World *>(env);
    Engine::Entity *entity = world->getEntityById(id);
    if (!entity)
    {
        std::cerr << "[Wasm] Entity with ID " << id << " not found" << std::endl;
    }
    else
    {
        if (entity->hasComponent<Engine::Components::TransformComponent>())
        {
            auto *transform = entity->getComponent<Engine::Components::TransformComponent>();
            transform->getTransform()->setPosition(glm::vec3(x, y, z));
            std::cout << "[Wasm] Set position of entity " << id << " to (" << x << ", " << y << ", " << z << ")" << std::endl;
        }
        else
        {
            std::cerr << "[Wasm] Entity with ID " << id << " does not have a TransformComponent" << std::endl;
        }
    }
    return nullptr;
}

void Engine::WasmBindings::bindAll(std::vector<wasmtime_extern_t> &wasmExports, Engine::WasmScriptingEngine *self)
{
    auto context = self->getContext();
    auto world = self->getWorld();

    std::unordered_map<std::string, wasmtime_extern_t> exportsMap;

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

    exportsMap["abort"] = makeFunc({wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32)}, {}, hostAbort);
    exportsMap["log"] = makeFunc({wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_I32)}, {}, hostLog);
    exportsMap["getEntityById"] = makeFunc({wasm_valtype_new(WASM_I32)}, {wasm_valtype_new(WASM_I32)}, getEntityById);
    exportsMap["setEntityPosition"] = makeFunc({wasm_valtype_new(WASM_I32), wasm_valtype_new(WASM_F32), wasm_valtype_new(WASM_F32), wasm_valtype_new(WASM_F32)}, {}, setEntityPosition);

    wasmExports.push_back(exportsMap["abort"]);
    wasmExports.push_back(exportsMap["setEntityPosition"]);
    wasmExports.push_back(exportsMap["log"]);
    wasmExports.push_back(exportsMap["getEntityById"]);
}
