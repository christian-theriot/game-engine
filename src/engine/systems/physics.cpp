#include <engine/systems/physics.hpp>
#include <engine/components/rigidbody.hpp>
#include <engine/components/transform.hpp>
#include <engine/world.hpp>

Engine::Systems::PhysicsSystem::PhysicsSystem()
    : physics(std::make_unique<Engine::Physics::PhysicsSystem>())
{
}
void Engine::Systems::PhysicsSystem::update(World *world, float deltaTime)
{
    physics->update(deltaTime);

    auto entities = world->getEntitiesWithComponent<Engine::Components::RigidbodyComponent>();

    for (auto *entity : entities)
    {
        auto *physicsComp = entity->getComponent<Engine::Components::RigidbodyComponent>();
        auto *transformComp = entity->getComponent<Engine::Components::TransformComponent>();

        if (physicsComp && transformComp)
        {
            auto *rigidbody = physicsComp->getRigidbody();
            transformComp->getTransform()->setPosition(rigidbody->getPosition());
        }
    }
}
Engine::Physics::PhysicsSystem *Engine::Systems::PhysicsSystem::getPhysics() const
{
    return physics.get();
}