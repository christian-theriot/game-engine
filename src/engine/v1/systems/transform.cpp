/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/systems/transform.hpp>
#include <engine/v1/components/transform.hpp>
#include <engine/v1/components/mesh.hpp>
#include <engine/v1/world.hpp>

void Engine::Systems::TransformSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Engine::Components::TransformComponent>();

    for (auto *entity : entities)
    {
        auto *transformComp = entity->getComponent<Engine::Components::TransformComponent>();
        transformComp->getTransform()->update();
    }
}