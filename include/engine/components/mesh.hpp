#ifndef __ENGINE_COMPONENTS_MESH_HPP
#define __ENGINE_COMPONENTS_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/component.hpp>
#include <engine/mesh.hpp>
#include <nlohmann/json.hpp>

namespace Engine::Components
{
    class MeshComponent : public Component
    {
        std::unique_ptr<Mesh> mesh;

    public:
        MeshComponent(std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>());
        Mesh *getMesh() const;
        const std::unique_ptr<Mesh> &getMeshPtr() const;
        void setMesh(std::unique_ptr<Mesh> mesh);
    };

    void to_json(nlohmann::json &j, const MeshComponent &meshComponent);
    void from_json(const nlohmann::json &j, MeshComponent &meshComponent);
}

#endif