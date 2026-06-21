/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/systems/scripting.hpp>
#include <engine/components/scripting.hpp>
#include <engine/world.hpp>
#include <engine/bindings.hpp>

Engine::Systems::LuaScriptingSystem::LuaScriptingSystem(World *world)
    : world(world)
{
    init();
}
Engine::Systems::LuaScriptingSystem::~LuaScriptingSystem()
{
    auto entities = world->getEntitiesWithComponent<Engine::Components::LuaScriptComponent>();
    for (auto *entity : entities)
    {
        auto *scriptComponent = entity->getComponent<Engine::Components::LuaScriptComponent>();
        if (!scriptComponent)
            continue;

        scriptComponent->onStart = sol::function();
        scriptComponent->onUpdate = sol::function();
        scriptComponent->self = sol::table();
    }
}
void Engine::Systems::LuaScriptingSystem::init()
{
    scriptingEngine = std::make_unique<Engine::LuaScriptingEngine>();
    LuaBindings().bindAll(scriptingEngine->state(), world);
}
void Engine::Systems::LuaScriptingSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Engine::Components::LuaScriptComponent>();

    for (auto *entity : entities)
    {
        auto *scriptComponent = entity->getComponent<Engine::Components::LuaScriptComponent>();

        if (!scriptComponent)
            continue;

        if (!scriptComponent->started)
        {
            if (scriptComponent->onStart.valid())
            {
                sol::protected_function_result result = scriptComponent->onStart(scriptComponent->self);
                if (!result.valid())
                {
                    sol::error err = result;
                    std::cerr << "[Lua] onStart error: " << err.what() << std::endl;
                }
            }
            scriptComponent->started = true;
        }

        if (scriptComponent->onUpdate.valid())
        {
            sol::protected_function_result result = scriptComponent->onUpdate(scriptComponent->self, deltaTime);
            if (!result.valid())
            {
                sol::error err = result;
                std::cerr << "[Lua] onUpdate error: " << err.what() << std::endl;
            }
        }
    }
}
bool Engine::Systems::LuaScriptingSystem::load(Engine::Components::LuaScriptComponent *component, const std::string &path)
{
    sol::state &lua = scriptingEngine->state();

    sol::protected_function_result result =
        lua.safe_script_file(path, sol::script_pass_on_error);

    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[Lua] Failed to load '" << path << "': " << err.what() << '\n';
        return false;
    }

    sol::table scriptTable = result; // safe now — validity checked

    component->scriptPath = path;
    component->self = lua.create_table();
    component->onStart = scriptTable["onStart"];
    component->onUpdate = scriptTable["onUpdate"];
    component->started = false;

    return true;
}

Engine::Systems::WasmScriptingSystem::WasmScriptingSystem(World *world)
    : world(world)
{
}
bool Engine::Systems::WasmScriptingSystem::load(Engine::Components::WasmScriptComponent *component, const std::string &path)
{
    component->scriptingEngine = std::make_unique<Engine::WasmScriptingEngine>();
    component->scriptingEngine->setWorld(world);

    if (!component->scriptingEngine->load(path))
    {
        std::cerr << "[Wasm] Failed to load '" << path << "'" << std::endl;
        component->scriptingEngine.reset();
        return false;
    }

    component->scriptPath = path;
    component->started = false;

    // TODO: add a hasExport(name) to WasmScriptingEngine and use it here instead of being optimistic and letting call catch missing functions
    component->hasOnStart = component->scriptingEngine->hasExport("onStart");
    component->hasOnUpdate = component->scriptingEngine->hasExport("onUpdate");

    return true;
}
void Engine::Systems::WasmScriptingSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Engine::Components::WasmScriptComponent>();

    for (auto *entity : entities)
    {
        auto *scriptComponent = entity->getComponent<Engine::Components::WasmScriptComponent>();

        if (!scriptComponent || !scriptComponent->scriptingEngine)
            continue;

        if (!scriptComponent->started)
        {
            if (scriptComponent->hasOnStart)
            {
                scriptComponent->scriptingEngine->call("onStart");
            }
            scriptComponent->started = true;
        }

        if (scriptComponent->hasOnUpdate)
        {
            scriptComponent->scriptingEngine->call("onUpdate", deltaTime);
        }
    }
}