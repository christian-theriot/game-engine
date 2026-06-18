#ifndef __EDITOR_SYSTEMS_TRANSFORM_CPP
#define __EDITOR_SYSTEMS_TRANSFORM_CPP

#include <editor/systems/transform.hpp>
#include <editor/components/transform.hpp>
#include <editor/components/mesh.hpp>
#include <editor/world.hpp>

void Editor::Systems::TransformSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Editor::Components::TransformComponent>();

    for (auto *entity : entities)
    {
        auto *transformComp = entity->getComponent<Components::TransformComponent>();
        transformComp->getTransform()->update();
    }
}

#endif