/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/mesh.hpp>
#include <engine/v1/primitives/line.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

enum OBJ_STATE
{
    INIT,
    LOADING_VERTICES,
    LOADING_TEXELS,
    LOADING_NORMALS,
    LOADING_FACES
};

void loadObject(const char *filename, std::vector<GLfloat> &vertices, std::vector<GLfloat> &uvs)
{
    vertices.clear();
    uvs.clear();

    std::ifstream file(filename);

    std::string name;

    std::vector<glm::vec3> vertexMap;
    std::vector<glm::vec2> uvMap;
    std::vector<glm::vec3> normalMap;

    OBJ_STATE objState = INIT;

    int j = 0;
    while (file.good())
    {
        j++;
        OBJ_STATE initialState = objState;
        std::string line;
        std::getline(file, line);

        if (line.find("o") == 0)
        {
            name = line.substr(2);
        }
        else if (line.find("v ") == 0)
        {
            objState = LOADING_VERTICES;
            std::istringstream iss(line.substr(2));
            float x, y, z;
            iss >> x;
            iss >> y;
            iss >> z;

            vertexMap.push_back(glm::vec3(x, y, z));
        }
        else if (line.find("vt ") == 0)
        {
            objState = LOADING_TEXELS;
            std::istringstream iss(line.substr(3));
            float u, v;
            iss >> u;
            iss >> v;

            uvMap.push_back(glm::vec2(u, v));
        }
        else if (line.find("vn ") == 0)
        {
            objState = LOADING_NORMALS;
            std::istringstream iss(line.substr(2));
            float x, y, z;
            iss >> x;
            iss >> y;
            iss >> z;

            normalMap.push_back(glm::vec3(x, y, z));
        }
        else if (line.find("f ") == 0)
        {
            bool isQuad = false;
            objState = LOADING_FACES;
            std::istringstream iss(line.substr(2));
            int vertex[4], uv[4], normal[4];
            char delim;

            iss >> vertex[0];
            iss >> delim;
            iss >> uv[0];
            iss >> delim;
            iss >> normal[0];

            iss >> vertex[1];
            iss >> delim;
            iss >> uv[1];
            iss >> delim;
            iss >> normal[1];

            iss >> vertex[2];
            iss >> delim;
            iss >> uv[2];
            iss >> delim;
            iss >> normal[2];

            if (iss.good())
            {
                iss >> vertex[3];
                iss >> delim;
                iss >> uv[3];
                iss >> delim;
                iss >> normal[3];
                isQuad = true;
            }

            for (int i = 0; i < 3; i++)
            {
                vertices.push_back(vertexMap[vertex[i] - 1].x);
                vertices.push_back(vertexMap[vertex[i] - 1].y);
                vertices.push_back(vertexMap[vertex[i] - 1].z);

                uvs.push_back(uvMap[uv[i] - 1].x);
                uvs.push_back(uvMap[uv[i] - 1].y);
            }
        }
    }
}
void initMesh(GLuint &VAO, GLuint &VBO, GLuint &UVBO, const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &uvs)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &UVBO);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GL_FLOAT), uvs.data(), GL_STATIC_DRAW);
}
Engine::Mesh::Mesh()
    : model(1.0f)
{
}
Engine::Mesh::Mesh(const char *filename, const Image &texture, const Material &material)
    : texture(texture),
      material(material),
      model(1.0f),
      filename(filename)
{
    loadObject(filename, vertices, uvs);
    initMesh(VAO, VBO, UVBO, vertices, uvs);
}
Engine::Mesh &Engine::Mesh::operator=(const Mesh &other)
{
    if (this != &other)
    {
        if (VAO != 0)
        {
            glDeleteVertexArrays(1, &VAO);
        }
        if (VBO != 0)
        {
            glDeleteBuffers(1, &VBO);
        }
        if (UVBO != 0)
        {
            glDeleteBuffers(1, &UVBO);
        }

        texture = other.texture;
        material = other.material;
        vertices = other.vertices;
        uvs = other.uvs;
        model = other.model;
        filename = other.filename;

        initMesh(VAO, VBO, UVBO, vertices, uvs);
    }
    return *this;
}
void Engine::Mesh::render(glm::mat4 mvp) const
{
    texture.use();

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
void Engine::Mesh::setMaterial(const Material &material)
{
    this->material = material;
}
void Engine::Mesh::setTexture(const Image &texture)
{
    this->texture = texture;
}
void Engine::Mesh::serialize(nlohmann::json &j) const
{
    j["type"] = "Mesh";
    j["file"] = filename;
    j["texture"] = texture;
    j["material"] = material;
    if (filename == "")
    {
        j["vertices"] = vertices;
        j["uvs"] = uvs;
    }
    j["model"] = model;
}
void Engine::Mesh::deserialize(const nlohmann::json &j)
{
    if (j.contains("file"))
    {
        filename = j["file"];
        loadObject(filename.c_str(), vertices, uvs);
        initMesh(VAO, VBO, UVBO, vertices, uvs);
    }
    else
    {
        vertices = j["vertices"].get<std::vector<GLfloat>>();
        uvs = j["uvs"].get<std::vector<GLfloat>>();
        initMesh(VAO, VBO, UVBO, vertices, uvs);
    }
    texture = j["texture"].get<Image>();
    material = j["material"].get<Material>();
    model = j["model"].get<glm::mat4>();
}

void Engine::to_json(nlohmann::json &j, const std::unique_ptr<Mesh> &mesh)
{
    mesh->serialize(j);
}
void Engine::from_json(const nlohmann::json &j, std::unique_ptr<Mesh> &mesh)
{
    if (j["type"] == "Mesh")
    {
        mesh = std::make_unique<Mesh>();
    }
    else if (j["type"] == "Line")
    {
        auto vertices = j["vertices"].get<std::vector<GLfloat>>();
        auto colors = j["colors"].get<std::vector<GLfloat>>();
        auto material = j["material"].get<Material>();
        mesh = std::make_unique<Primitives::Line>(vertices, colors, material);
    }
    else
    {
        std::cout << "Unknown mesh type: " << j["type"] << std::endl;
        return;
    }
    mesh->deserialize(j);
}