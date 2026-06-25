#ifndef __ENGINE_V2_RESOURCES_TEXTURE_HPP
#define __ENGINE_V2_RESOURCES_TEXTURE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/core/gl-handle.hpp>
#include <engine/v2/core/result.hpp>
#include <engine/v2/serialization/serializable.hpp>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine::Resources
{
    class Texture : public Serialization::ISerializable
    {
        Core::TextureHandle texture;
        std::string path;

        inline Texture(Core::TextureHandle texture, std::string path)
            : texture(std::move(texture)),
              path(std::move(path)) {}

    public:
        static Core::Result<Texture> load(const std::string &path);

        inline GLuint id() const { return texture.get(); }
        inline const std::string &getPath() const { return path; }
        inline void use() const { glBindTexture(GL_TEXTURE_2D, texture.get()); }
    };

    void to_json(nlohmann::json &j, const Texture &texture);
    void from_json(const nlohmann::json &j, Texture &texture);

    void to_json(nlohmann::json &j, const Core::Result<Texture> &texture);
    void from_json(const nlohmann::json &j, Core::Result<Texture> &texture);
}

#endif