#ifndef __ENGINE_MESH_HPP
#define __ENGINE_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/renderable.hpp>
#include <engine/v1/material.hpp>
#include <engine/v1/image.hpp>
#include <engine/v1/clock.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace glm
{
    inline void to_json(nlohmann::json &j, const glm::vec4 &vec)
    {
        j = nlohmann::json{vec.x, vec.y, vec.z, vec.w};
    }
    inline void from_json(const nlohmann::json &j, glm::vec4 &vec)
    {
        vec.x = j[0];
        vec.y = j[1];
        vec.z = j[2];
        vec.w = j[3];
    }

    inline void to_json(nlohmann::json &j, const glm::mat4 &mat)
    {
        j = {mat[0], mat[1], mat[2], mat[3]};
    }
    inline void from_json(const nlohmann::json &j, glm::mat4 &mat)
    {
        mat[0] = j.at(0).get<glm::vec4>();
        mat[1] = j.at(1).get<glm::vec4>();
        mat[2] = j.at(2).get<glm::vec4>();
        mat[3] = j.at(3).get<glm::vec4>();
    }
}

namespace Engine
{
    class Mesh : public Renderable
    {
    protected:
        Image texture;
        Material material;
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> uvs;
        glm::mat4 model;
        GLuint VAO, VBO, UVBO;
        std::string filename;

    public:
        Mesh();
        Mesh(const char *filename, const Image &texture = "assets/textures/checkerboard.png", const Material &material = "assets/shaders/texture");
        virtual ~Mesh() = default;

        Mesh &operator=(const Mesh &mesh);
        void render(glm::mat4 mvp) const;

        void setMaterial(const Material &material);
        void setTexture(const Image &texture);

        virtual void serialize(nlohmann::json &j) const;
        virtual void deserialize(const nlohmann::json &j);
    };

    void to_json(nlohmann::json &j, const std::unique_ptr<Mesh> &mesh);
    void from_json(const nlohmann::json &j, std::unique_ptr<Mesh> &mesh);
}

#endif