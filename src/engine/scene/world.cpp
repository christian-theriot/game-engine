#include <engine/scene/world.hpp>
#include <engine/scene/systems/physics.hpp>
#include <engine/scene/systems/script.hpp>
#include <algorithm>
#include <iostream>

uint64_t Engine::Scene::World::nextEntityId = 1;

Engine::Scene::Entity *Engine::Scene::World::createEntity()
{
    auto entity = std::make_unique<Entity>();
    Entity *entityPtr = entity.get();
    entityPtr->setId(nextEntityId++);
    entities.push_back(std::move(entity));
    return entityPtr;
}
void Engine::Scene::World::destroyEntity(Entity *entity)
{
    auto it = std::find_if(entities.begin(), entities.end(), [entity](const std::unique_ptr<Entity> &e)
                           { return e && e.get() == entity; });

    if (it != entities.end())
    {
        entities.erase(it);
    }
}
Engine::Scene::Entity *Engine::Scene::World::getEntityById(uint64_t id) const
{
    auto it = std::find_if(entities.begin(), entities.end(), [id](const std::unique_ptr<Entity> &e)
                           { return e && e->getId() == id; });

    if (it != entities.end())
    {
        return it->get();
    }

    return nullptr;
}
void Engine::Scene::World::update(float deltaTime)
{
    if (hasSystem<Systems::Physics>())
        getSystem<Systems::Physics>()->update(this, deltaTime);
    if (hasSystem<Systems::LuaScript>())
        getSystem<Systems::LuaScript>()->update(this, deltaTime);
    if (hasSystem<Systems::WasmScript>())
        getSystem<Systems::WasmScript>()->update(this, deltaTime);
}
void Engine::Scene::to_json(nlohmann::json &j, const World &world)
{
    j["entities"] = nlohmann::json::array();
    for (const auto &entity : world.getEntities())
    {
        j["entities"].push_back(*entity);
    }

    j["systems"] = nlohmann::json::array();
    if (world.hasSystem<Systems::Physics>())
    {
        j["systems"].push_back("Physics");
    }
    if (world.hasSystem<Systems::LuaScript>())
    {
        j["systems"].push_back("Lua");
    }
    if (world.hasSystem<Systems::WasmScript>())
    {
        j["systems"].push_back("Wasm");
    }
}
void Engine::Scene::from_json(const nlohmann::json &j, World &world)
{
    Systems::Physics *physics = nullptr;
    Systems::LuaScript *lua = nullptr;
    Systems::WasmScript *wasm = nullptr;

    for (const auto &systemJson : j.at("systems"))
    {
        const std::string systemType = systemJson.get<std::string>();
        if (systemType == "Physics")
        {
            physics = world.addSystem<Systems::Physics>();
        }
        else if (systemType == "Lua")
        {
            lua = world.addSystem<Systems::LuaScript>();
        }
        else if (systemType == "Wasm")
        {
            wasm = world.addSystem<Systems::WasmScript>();
        }
        else
        {
            std::cerr << "Unknown system type: " << systemType << std::endl;
        }
    }

    for (const auto &entityJson : j.at("entities"))
    {
        Entity *entity = world.createEntity();
        entityJson.get_to(*entity);

        if (physics)
        {
            physics->add(entity);
        }
    }
}