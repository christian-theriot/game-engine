#include <editor/mesh-w-color.hpp>
#include <iostream>

void initMeshWithColor(
    GLuint &CBO,
    const std::vector<GLfloat> &colors)
{
    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GL_FLOAT), colors.data(), GL_STATIC_DRAW);
}

MeshWithColor::MeshWithColor(const Material &material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors)
    : Mesh(material, vertices),
      colors(colors)
{
    initMeshWithColor(CBO, colors);
}

MeshWithColor::MeshWithColor(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const Material &material)
    : Mesh(material, vertices),
      colors(colors)
{
    initMeshWithColor(CBO, colors);
}

MeshWithColor::MeshWithColor(const char *material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors)
    : Mesh(material, vertices),
      colors(colors)
{
    initMeshWithColor(CBO, colors);
}

MeshWithColor::MeshWithColor(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> colors, const char *material)
    : Mesh(material, vertices),
      colors(colors)
{
    initMeshWithColor(CBO, colors);
}

MeshWithColor::~MeshWithColor() {}

void MeshWithColor::render(glm::mat4 mvp) const
{
    mvp = mvp * model;
    glUseProgram(material.get());

    GLuint matrixID = glGetUniformLocation(material.get(), "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glDisableVertexAttribArray(0);
}