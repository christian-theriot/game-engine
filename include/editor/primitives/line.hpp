#ifndef __EDITOR_PRIMITIVES_LINE_HPP
#define __EDITOR_PRIMITIVES_LINE_HPP

#include <editor/material.hpp>
#include <editor/mesh.hpp>
#include <glm/glm.hpp>

namespace Editor::Primitives
{
    class Line : public Mesh
    {
        Material material;
        std::vector<GLfloat> colors;
        GLuint CBO;

    public:
        Line(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const Editor::Material &material = "assets/shaders/color");

        void render(glm::mat4 mvp) const;
    };
}

#endif