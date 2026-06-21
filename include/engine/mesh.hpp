#ifndef __ENGINE_MESH_HPP
#define __ENGINE_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/renderable.hpp>
#include <engine/material.hpp>
#include <engine/image.hpp>
#include <engine/clock.hpp>
#include <glm/glm.hpp>
#include <memory>

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

    public:
        Mesh(const char *filename, const Image &texture = "assets/textures/checkerboard.png", const Material &material = "assets/shaders/texture");
        virtual ~Mesh() = default;

        void render(glm::mat4 mvp) const;

        void setTexture(const Image &texture);
    };
}

#endif