#include <engine/resources/texture.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Engine::Core::Result<Engine::Resources::Texture> Engine::Resources::Texture::load(const std::string &path)
{
    Core::TextureHandle textureID;
    GLuint rawTextureID;
    int width, height, numChannels;
    unsigned char *data = stbi_load(("assets/textures/" + path).c_str(), &width, &height, &numChannels, 0);

    if (data == nullptr)
    {
        return Core::Fail<Texture>("Failed to load file: " + path);
    }

    glGenTextures(1, &rawTextureID);
    textureID.reset(rawTextureID);

    glBindTexture(GL_TEXTURE_2D, textureID.get());

    switch (numChannels)
    {
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    default:
        stbi_image_free(data);
        return Core::Fail<Texture>("Unsupported number of channels: " + std::to_string(numChannels) + " in file: " + path);
    }

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return Core::Ok<Texture>(Texture(std::move(textureID), path));
}

void Engine::Resources::to_json(nlohmann::json &j, const Engine::Resources::Texture &texture)
{
    j["path"] = texture.getPath();
}
void Engine::Resources::from_json(const nlohmann::json &j, Engine::Resources::Texture &texture)
{
    auto result = Engine::Resources::Texture::load(j.at("path").get<std::string>());
    if (result.ok())
    {
        texture = std::move(result.value());
    }
    else
    {
        throw result.error();
    }
}
void Engine::Resources::to_json(nlohmann::json &j, const Engine::Core::Result<Engine::Resources::Texture> &texture)
{
    if (texture.ok())
    {
        j["result"]["ok"] = true;
        j["result"]["texture"] = texture.value();
    }
    else
    {
        j["result"]["ok"] = false;
        j["result"]["error"] = texture.error().what();
    }
}
void Engine::Resources::from_json(const nlohmann::json &j, Engine::Core::Result<Engine::Resources::Texture> &texture)
{
    if (j.at("result").at("ok").get<bool>())
    {
        const std::string path = j.at("result").at("texture").at("path").get<std::string>();
        auto loaded = Engine::Resources::Texture::load(path);

        texture = std::move(loaded);
    }
    else
    {
        std::string errorMessage = j.at("result").at("error").get<std::string>();
        texture = Engine::Core::Fail<Engine::Resources::Texture>(errorMessage);
    }
}