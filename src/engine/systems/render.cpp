/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/systems/render.hpp>
#include <engine/components/mesh.hpp>
#include <engine/components/transform.hpp>
#include <engine/world.hpp>
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