#include <engine/world.hpp>
#include <iostream>

Engine::Entity *Engine::World::createEntity()
{
    auto entity = std::make_shared<Entity>();
    entities.push_back(entity);
    return entity.get();
}
void Engine::World::destroyEntity(Entity *entity)
{
    auto item = std::find_if(entities.begin(), entities.end(),
                             [entity](const std::shared_ptr<Entity> &e)
                             { return e.get() == entity; });

    if (item != entities.end())
    {
        entities.erase(item);
    }
}
const std::vector<std::shared_ptr<Engine::Entity>> &Engine::World::getEntities() const
{
    return entities;
}
void Engine::World::update(float deltaTime)
{
    for (auto &[typeHash, system] : systems)
    {
        system->update(this, deltaTime);
    }
}