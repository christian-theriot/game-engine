/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/components/mesh.hpp>

Engine::Components::MeshComponent::MeshComponent(std::unique_ptr<Mesh> mesh)
    : mesh(std::move(mesh))
{
}
Engine::Mesh *Engine::Components::MeshComponent::getMesh() const
{
    return mesh.get();
}
void Engine::Components::MeshComponent::setMesh(std::unique_ptr<Mesh> m)
{
    mesh = std::move(m);
}