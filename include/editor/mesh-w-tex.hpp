#ifndef __MESH_W_TEX_HPP
#define __MESH_W_TEX_HPP

#include <editor/material.hpp>
#include <editor/mesh.hpp>
#include <glm/glm.hpp>
#include <vector>

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

#endif