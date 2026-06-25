#ifndef __ENGINE_V2_SCENE_COMPONENTS_MESH_HPP
#define __ENGINE_V2_SCENE_COMPONENTS_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/serialization/serializable.hpp>
#include <glad/glad.h>
#include <vector>
#include <nlohmann/json.hpp>

namespace Engine::Resources
{
    struct Mesh : public Serialization::ISerializable
    {
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> uvs;
        std::vector<GLfloat> colors;
    };

    void to_json(nlohmann::json &j, const Mesh &mesh);
    void from_json(const nlohmann::json &j, Mesh &mesh);
}

#endif