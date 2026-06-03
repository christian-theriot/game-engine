#include <mesh-w-tex.hpp>
#include <iostream>

void initMeshWithUV(
    GLuint &UVBO,
    const std::vector<GLfloat> &uv)
{
    glGenBuffers(1, &UVBO);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(GL_FLOAT), uv.data(), GL_STATIC_DRAW);
}

MeshWithTexture::MeshWithTexture(const Material &material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> uv)
    : Mesh(material, vertices),
      uv(uv)
{
    initMeshWithUV(UVBO, uv);
}

MeshWithTexture::MeshWithTexture(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> uv, const Material &material)
    : Mesh(material, vertices),
      uv(uv)
{
    initMeshWithUV(UVBO, uv);
}

MeshWithTexture::MeshWithTexture(const char *material, const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> uv)
    : Mesh(material, vertices),
      uv(uv)
{
    initMeshWithUV(UVBO, uv);
}

MeshWithTexture::MeshWithTexture(const std::initializer_list<GLfloat> vertices, const std::initializer_list<GLfloat> uv, const char *material)
    : Mesh(material, vertices),
      uv(uv)
{
    initMeshWithUV(UVBO, uv);
}

MeshWithTexture::~MeshWithTexture() {}

void MeshWithTexture::render(glm::mat4 mvp) const
{
    mvp = mvp * model;
    glUseProgram(material.get());

    GLuint matrixID = glGetUniformLocation(material.get(), "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glDisableVertexAttribArray(0);
}