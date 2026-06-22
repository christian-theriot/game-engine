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
const std::unique_ptr<Engine::Mesh> &Engine::Components::MeshComponent::getMeshPtr() const
{
    return mesh;
}
void Engine::Components::MeshComponent::setMesh(std::unique_ptr<Mesh> m)
{
    mesh = std::move(m);
}
void Engine::Components::to_json(nlohmann::json &j, const MeshComponent &meshComponent)
{
    j["mesh"] = meshComponent.getMeshPtr();
}
void Engine::Components::from_json(const nlohmann::json &j, MeshComponent &meshComponent)
{
    meshComponent.setMesh(j["mesh"].get<std::unique_ptr<Mesh>>());
}