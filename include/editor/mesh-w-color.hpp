#ifndef __MESH_W_COLOR_HPP
#define __MESH_W_COLOR_HPP

#include <editor/material.hpp>
#include <editor/mesh.hpp>
#include <glm/glm.hpp>
#include <vector>

class MeshWithColor : public Mesh
{
    std::vector<GLfloat> colors;
    GLuint CBO;

public:
    MeshWithColor(const Material &material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors);
    MeshWithColor(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const Material &material);
    MeshWithColor(const char *material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors);
    MeshWithColor(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const char *material);
    ~MeshWithColor();

    void render(glm::mat4 mvp) const;
};

#endif