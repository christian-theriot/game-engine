#ifndef __ENGINE_V2_SCENE_COMPONENTS_MESH_HPP
#define __ENGINE_V2_SCENE_COMPONENTS_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/resources/mesh.hpp>
#include <engine/v2/scene/component.hpp>
#include <nlohmann/json.hpp>

namespace Engine::Scene::Components
{
    class Mesh : public Component
    {
        Resources::Mesh mesh;

    public:
        inline Mesh(const Resources::Mesh &mesh = {}) : mesh(mesh) {}
        inline Mesh(const std::string &path) : mesh({}) { mesh.path = path; }

        inline Resources::Mesh &getMesh() { return mesh; }
        inline const Resources::Mesh &getMesh() const { return mesh; }
        inline void setMesh(const Resources::Mesh &mesh) { this->mesh = mesh; }
        std::string type() const override { return "Mesh"; }
    };

    void to_json(nlohmann::json &j, const Mesh &mesh);
    void from_json(const nlohmann::json &j, Mesh &mesh);
}

#endif