#ifndef __ENGINE_PRIMITIVES_LINE_HPP
#define __ENGINE_PRIMITIVES_LINE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/material.hpp>
#include <engine/mesh.hpp>
#include <glm/glm.hpp>

namespace Engine::Primitives
{
    class Line : public Mesh
    {
        Material material;
        std::vector<GLfloat> colors;
        GLuint CBO;

    public:
        Line(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const Engine::Material &material = "assets/shaders/color");

        void render(glm::mat4 mvp) const;
    };
}

#endif