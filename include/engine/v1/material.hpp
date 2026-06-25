#ifndef __ENGINE_MATERIAL_HPP
#define __ENGINE_MATERIAL_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine
{
    class Material
    {
        GLuint program;
        std::string filename;

    public:
        Material();
        Material(const Material &material);
        Material(const char *filename);

        Material &operator=(const Material &material);

        GLuint get() const;
        const std::string &getFilename() const;
    };

    void to_json(nlohmann::json &j, const Material &material);
    void from_json(const nlohmann::json &j, Material &material);
}

#endif