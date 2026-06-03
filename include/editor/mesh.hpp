#ifndef __MESH_HPP
#define __MESH_HPP

#include <editor/material.hpp>
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
protected:
    Material material;
    std::vector<GLfloat> vertices;
    glm::mat4 model;
    GLuint VAO, VBO;

public:
    Mesh(const Material &material, const std::initializer_list<GLfloat> vertices);
    Mesh(const std::initializer_list<GLfloat> vertices, const Material &material);
    Mesh(const char *material, const std::initializer_list<GLfloat> vertices);
    Mesh(const std::initializer_list<GLfloat> vertices, const char *material);
    ~Mesh();

    virtual void render(glm::mat4 mvp) const;
};

#endif