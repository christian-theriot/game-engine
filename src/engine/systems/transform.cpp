#include <engine/systems/transform.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/mesh.hpp>
#include <engine/world.hpp>

void Engine::Systems::TransformSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Engine::Components::TransformComponent>();

    for (auto *entity : entities)
    {
        auto *transformComp = entity->getComponent<Engine::Components::TransformComponent>();
        transformComp->getTransform()->update();
    }
}