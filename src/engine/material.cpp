/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/material.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Engine::Material::Material()
    : program(0)
{
}

Engine::Material::Material(const Engine::Material &material)
    : program(material.program),
      filename(material.filename)
{
}

Engine::Material::Material(const char *filename)
    : filename(filename)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(std::string(filename) + "/vertex.glsl");
    if (vertexShaderStream.is_open())
    {
        std::stringstream ss;
        ss << vertexShaderStream.rdbuf();
        vertexShaderCode = ss.str();
        vertexShaderStream.close();
    }
    else
    {
        std::cout << "Impossible to open " << filename << "/vertex.glsl" << std::endl;
        return;
    }

    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(std::string(filename) + "/fragment.glsl");
    if (fragmentShaderStream.is_open())
    {
        std::stringstream ss;
        ss << fragmentShaderStream.rdbuf();
        fragmentShaderCode = ss.str();
        fragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    char const *vertexShaderPtr = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexShaderPtr, nullptr);
    glCompileShader(vertexShaderID);

    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    char const *fragmentShaderPtr = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentShaderPtr, nullptr);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
        std::cout << &fragmentShaderErrorMessage[0] << std::endl;
    }

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
        std::cout << &programErrorMessage[0] << std::endl;
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    this->program = programID;
}

Engine::Material &Engine::Material::operator=(const Material &material)
{
    if (this != &material)
    {
        if (program != 0)
        {
            glDeleteProgram(program);
        }
        program = material.program;
        filename = material.filename;
    }
    return *this;
}

GLuint Engine::Material::get() const
{
    return this->program;
}
const std::string &Engine::Material::getFilename() const
{
    return filename;
}

void Engine::to_json(nlohmann::json &j, const Material &material)
{
    j["file"] = material.getFilename();
}
void Engine::from_json(const nlohmann::json &j, Material &material)
{
    material = Material(j.at("file").get<std::string>().c_str());
}