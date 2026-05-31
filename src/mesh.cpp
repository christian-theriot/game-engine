#include <mesh.hpp>
#include <iostream>

void initMesh(GLuint &VAO, GLuint &VBO, const std::vector<GLfloat> &vertices)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), vertices.data(), GL_STATIC_DRAW);
}

Mesh::Mesh(const Material &material, const std::initializer_list<GLfloat> vertices)
    : material(material),
      vertices(vertices),
      model(1.0f)
{
    initMesh(VAO, VBO, vertices);
}

Mesh::Mesh(const std::initializer_list<GLfloat> vertices, const Material &material)
    : material(material),
      vertices(vertices),
      model(1.0f)
{
    initMesh(VAO, VBO, vertices);
}

Mesh::Mesh(const char *material, const std::initializer_list<GLfloat> vertices)
    : material(material),
      vertices(vertices),
      model(1.0f)
{
    initMesh(VAO, VBO, vertices);
}

Mesh::Mesh(const std::initializer_list<GLfloat> vertices, const char *material)
    : material(material),
      vertices(vertices),
      model(1.0f)
{
    initMesh(VAO, VBO, vertices);
}

Mesh::~Mesh() {}

void Mesh::render(glm::mat4 mvp) const
{
    mvp = mvp * model;
    glUseProgram(material.get());

    GLuint matrixID = glGetUniformLocation(material.get(), "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}