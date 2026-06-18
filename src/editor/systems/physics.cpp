#include <editor/systems/physics.hpp>
#include <editor/components/rigidbody.hpp>
#include <editor/components/transform.hpp>
#include <editor/world.hpp>

Editor::Systems::PhysicsSystem::PhysicsSystem()
    : physics(std::make_unique<Editor::Physics::PhysicsSystem>())
{
}
void Editor::Systems::PhysicsSystem::update(World *world, float deltaTime)
{
    physics->update(deltaTime);

    auto entities = world->getEntitiesWithComponent<Editor::Components::RigidbodyComponent>();

    for (auto *entity : entities)
    {
        auto *physicsComp = entity->getComponent<Components::RigidbodyComponent>();
        auto *transformComp = entity->getComponent<Components::TransformComponent>();

        if (physicsComp && transformComp)
        {
            auto *rigidbody = physicsComp->getRigidbody();
            transformComp->getTransform()->setPosition(rigidbody->getPosition());
        }
    }
}
Editor::Physics::PhysicsSystem *Editor::Systems::PhysicsSystem::getPhysics() const
{
    return physics.get();
}