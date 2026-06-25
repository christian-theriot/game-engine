/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/primitives/line.hpp>

void initLineMesh(GLuint &VAO, GLuint &VBO, GLuint &CBO, const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &colors)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
}

Engine::Primitives::Line::Line(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &colors, const Engine::Material &material)
    : Engine::Mesh(),
      colors(colors)
{
    this->setMaterial(material);
    this->vertices = vertices;
    initLineMesh(VAO, VBO, CBO, vertices, colors);
}
void Engine::Primitives::Line::render(glm::mat4 mvp) const
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

    glDrawArrays(GL_LINES, 0, vertices.size() / 3);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Engine::Primitives::Line::serialize(nlohmann::json &j) const
{
    Mesh::serialize(j);
    j["type"] = "Line";
    j["colors"] = colors;
}
void Engine::Primitives::Line::deserialize(const nlohmann::json &j)
{
    colors = j["colors"].get<std::vector<GLfloat>>();

    initLineMesh(VAO, VBO, CBO, vertices, colors);
}