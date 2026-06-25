#include <engine/v2/resources/shader.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

Engine::Core::Result<Engine::Resources::Shader> Engine::Resources::Shader::load(const std::string &path)
{
    Core::ShaderHandle vertexShaderID(glCreateShader(GL_VERTEX_SHADER));
    Core::ShaderHandle fragmentShaderID(glCreateShader(GL_FRAGMENT_SHADER));

    std::string vertexShaderCode;
    std::ifstream vertexShaderStream("assets/shaders/" + path + "/vertex.glsl");
    if (vertexShaderStream.is_open())
    {
        std::stringstream ss;
        ss << vertexShaderStream.rdbuf();
        vertexShaderCode = ss.str();
        vertexShaderStream.close();
    }
    else
    {
        std::cout << "Impossible to open assets/shaders/" << path << "/vertex.glsl" << std::endl;
        return Core::Fail<Shader>("Failed to open vertex shader");
    }

    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream("assets/shaders/" + path + "/fragment.glsl");
    if (fragmentShaderStream.is_open())
    {
        std::stringstream ss;
        ss << fragmentShaderStream.rdbuf();
        fragmentShaderCode = ss.str();
        fragmentShaderStream.close();
    }
    else
    {
        std::cout << "Impossible to open assets/shaders/" << path << "/fragment.glsl" << std::endl;
        return Core::Fail<Shader>("Failed to open fragment shader");
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    char const *vertexShaderPtr = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID.get(), 1, &vertexShaderPtr, nullptr);
    glCompileShader(vertexShaderID.get());

    glGetShaderiv(vertexShaderID.get(), GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID.get(), infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
        std::cout << &vertexShaderErrorMessage[0] << std::endl;
    }

    char const *fragmentShaderPtr = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID.get(), 1, &fragmentShaderPtr, nullptr);
    glCompileShader(fragmentShaderID.get());

    glGetShaderiv(fragmentShaderID.get(), GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID.get(), infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
        std::cout << &fragmentShaderErrorMessage[0] << std::endl;
    }

    Core::ProgramHandle programID(glCreateProgram());
    glAttachShader(programID.get(), vertexShaderID.get());
    glAttachShader(programID.get(), fragmentShaderID.get());
    glLinkProgram(programID.get());

    glGetProgramiv(programID.get(), GL_LINK_STATUS, &result);
    glGetProgramiv(programID.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0)
    {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID.get(), infoLogLength, nullptr, &programErrorMessage[0]);
        std::cout << &programErrorMessage[0] << std::endl;
    }

    glDetachShader(programID.get(), vertexShaderID.get());
    glDetachShader(programID.get(), fragmentShaderID.get());

    return Core::Ok(Shader(std::move(programID), path));
}

void Engine::Resources::to_json(nlohmann::json &j, const Engine::Resources::Shader &shader)
{
    j["path"] = shader.getPath();
}
void Engine::Resources::from_json(const nlohmann::json &j, Engine::Resources::Shader &shader)
{
    auto result = Engine::Resources::Shader::load(j.at("path").get<std::string>());
    if (result.ok())
    {
        shader = std::move(result.value());
    }
    else
    {
        throw result.error();
    }
}
void Engine::Resources::to_json(nlohmann::json &j, const Engine::Core::Result<Engine::Resources::Shader> &shader)
{
    if (shader.ok())
    {
        j["result"]["ok"] = true;
        j["result"]["shader"] = shader.value();
    }
    else
    {
        j["result"]["ok"] = false;
        j["result"]["error"] = shader.error().what();
    }
}
void Engine::Resources::from_json(const nlohmann::json &j, Engine::Core::Result<Engine::Resources::Shader> &shader)
{
    if (j.at("result").at("ok").get<bool>())
    {
        const std::string path = j.at("result").at("shader").at("path").get<std::string>();
        auto loaded = Engine::Resources::Shader::load(path);

        shader = std::move(loaded);
    }
    else
    {
        std::string errorMessage = j.at("result").at("error").get<std::string>();
        shader = Engine::Core::Fail<Engine::Resources::Shader>(errorMessage);
    }
}