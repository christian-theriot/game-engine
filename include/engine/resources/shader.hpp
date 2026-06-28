#ifndef __ENGINE_RESOURCES_SHADER_HPP
#define __ENGINE_RESOURCES_SHADER_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/core/gl-handle.hpp>
#include <engine/core/result.hpp>
#include <engine/serialization/serializable.hpp>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine::Resources
{
    class Shader : public Serialization::ISerializable
    {
        Core::ProgramHandle program;
        std::string path;

        inline Shader(Core::ProgramHandle program, std::string path)
            : program(std::move(program)),
              path(std::move(path)) {}

    public:
        static Core::Result<Shader> load(const std::string &path);

        inline GLuint id() const { return program.get(); }
        inline const std::string &getPath() const { return path; }
        inline void use() const { glUseProgram(program.get()); }
    };

    void to_json(nlohmann::json &j, const Shader &shader);
    void from_json(const nlohmann::json &j, Shader &shader);

    void to_json(nlohmann::json &j, const Core::Result<Shader> &shader);
    void from_json(const nlohmann::json &j, Core::Result<Shader> &shader);
}
#endif