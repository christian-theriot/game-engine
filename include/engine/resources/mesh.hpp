#ifndef __ENGINE_RESOURCES_MESH_HPP
#define __ENGINE_RESOURCES_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/serialization/serializable.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine::Resources
{
    struct Mesh : public Serialization::ISerializable
    {
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> uvs;
        std::vector<GLfloat> colors;
        std::vector<GLuint> indices;

        std::string path;
    };

    void to_json(nlohmann::json &j, const Mesh &mesh);
    void from_json(const nlohmann::json &j, Mesh &mesh);
}

#endif