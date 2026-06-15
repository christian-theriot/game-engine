#ifndef __EDITOR_PRIMITIVES_LINE_HPP
#define __EDITOR_PRIMITIVES_LINE_HPP

#include <editor/renderable.hpp>
#include <editor/material.hpp>
#include <glm/glm.hpp>

namespace Editor::Primitives
{
    class Line : public Renderable
    {
        Material material;
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> colors;
        glm::mat4 model;
        GLuint VAO, VBO, CBO;

    public:
        Line(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const Editor::Material &material = "assets/shaders/color");

        void render(glm::mat4 mvp) const;
    };
}

#endif