#ifndef __EDITOR_MESH_W_TEX_HPP
#define __EDITOR_MESH_W_TEX_HPP

#include <editor/material.hpp>
#include <editor/mesh.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace Editor
{
    class MeshWithTexture : public Mesh
    {
        std::vector<GLfloat> uv;
        GLuint UVBO;

    public:
        MeshWithTexture(const Material &material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors);
        MeshWithTexture(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const Material &material);
        MeshWithTexture(const char *material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors);
        MeshWithTexture(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const char *material);
        ~MeshWithTexture();

        void render(glm::mat4 mvp) const;
    };
}

#endif