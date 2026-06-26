#include <engine/v2/scene/world.hpp>
#include <engine/v2/scene/systems/clock.hpp>
#include <engine/v2/scene/systems/events.hpp>
#include <engine/v2/scene/systems/input.hpp>
#include <engine/v2/scene/systems/physics.hpp>
#include <engine/v2/scene/systems/window.hpp>
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
void Engine::Scene::World::update()
{
    auto clock = getSystem<Systems::Clock>();
    float deltaTime = clock ? clock->getDeltaTime() : 0.f;

    // We want to maintain a relatively strict order of system updates to ensure that the game logic behaves as expected. The order is as follows:
    // 1. Clock: Update the clock to get the delta time for this frame.
    // 2. Physics: Update the physics system to simulate physics for this frame.
    // 3. Input: Process input events to update the state of the game based on user interactions.
    // 4. EventBus: Process any events that have been queued up during the frame, allowing systems to react to changes.
    // 5. Window: Refresh the window to display the latest frame and handle window events.
    getSystem<Systems::Clock>()->update(this, deltaTime);
    getSystem<Systems::Physics>()->update(this, deltaTime);
    getSystem<Systems::InputSystem>()->update(this, deltaTime);
    getSystem<Systems::EventBus>()->update(this, deltaTime);
    getSystem<Systems::Window>()->update(this, deltaTime);
}
void Engine::Scene::to_json(nlohmann::json &j, const World &world)
{
    j["entities"] = nlohmann::json::array();
    for (const auto &entity : world.getEntities())
    {
        j["entities"].push_back(*entity);
    }

    j["systems"] = nlohmann::json::array();
    if (world.hasSystem<Systems::Clock>())
    {
        j["systems"].push_back("Clock");
    }
    if (world.hasSystem<Systems::EventBus>())
    {
        j["systems"].push_back("Events");
    }
    if (world.hasSystem<Systems::Physics>())
    {
        j["systems"].push_back("Physics");
    }
    if (world.hasSystem<Systems::Window>())
    {
        j["systems"].push_back("Window");
    }
    if (world.hasSystem<Systems::InputSystem>())
    {
        j["systems"].push_back("Input");
    }
}
void Engine::Scene::from_json(const nlohmann::json &j, World &world)
{
    Systems::Physics *physics = nullptr;
    for (const auto &systemJson : j.at("systems"))
    {
        const std::string systemType = systemJson.get<std::string>();
        if (systemType == "Clock")
        {
            world.addSystem<Systems::Clock>();
        }
        else if (systemType == "Events")
        {
            world.addSystem<Systems::EventBus>();
        }
        else if (systemType == "Input")
        {
            world.addSystem<Systems::InputSystem>(world.getSystem<Systems::Window>(), world.getSystem<Systems::EventBus>());
        }
        else if (systemType == "Window")
        {
            world.addSystem<Systems::Window>();
        }
        else if (systemType == "Physics")
        {
            physics = world.addSystem<Systems::Physics>();
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