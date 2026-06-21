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
    ScriptBindings::bindAll(scriptingEngine->state(), world);
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