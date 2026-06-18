#include <editor/systems/render.hpp>
#include <editor/components/mesh.hpp>
#include <editor/components/transform.hpp>
#include <editor/world.hpp>
#include <iostream>

void Editor::Systems::RenderSystem::update(World *world, float deltaTime)
{
    auto entities = world->getEntitiesWithComponent<Editor::Components::MeshComponent>();

    for (auto *entity : entities)
    {
        auto *meshComp = entity->getComponent<Components::MeshComponent>();
        auto *transformComp = entity->getComponent<Components::TransformComponent>();

        if (meshComp && transformComp)
        {
            glm::mat4 mvp = viewProjection * transformComp->getTransform()->getWorldMatrix();
            meshComp->getMesh()->render(mvp);
        }
    }
}
void Editor::Systems::RenderSystem::setViewProjection(const glm::mat4 &view)
{
    viewProjection = view;
}