/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/systems/render.hpp>
#include <engine/v1/components/mesh.hpp>
#include <engine/v1/components/transform.hpp>
#include <engine/v1/world.hpp>
#include <iostream>

void Engine::Systems::RenderSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Engine::Components::MeshComponent>();

    for (auto *entity : entities)
    {
        auto *meshComp = entity->getComponent<Engine::Components::MeshComponent>();
        auto *transformComp = entity->getComponent<Engine::Components::TransformComponent>();

        if (meshComp && transformComp)
        {
            glm::mat4 mvp = viewProjection * transformComp->getTransform()->getWorldMatrix();
            meshComp->getMesh()->render(mvp);
        }
    }
}
void Engine::Systems::RenderSystem::setViewProjection(const glm::mat4 &view)
{
    viewProjection = view;
}