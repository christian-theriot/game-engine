#ifndef __EDITOR_MESH_HPP
#define __EDITOR_MESH_HPP

#include <editor/renderable.hpp>
#include <editor/material.hpp>
#include <editor/image.hpp>
#include <editor/clock.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Editor
{
    class Mesh : public Renderable
    {
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