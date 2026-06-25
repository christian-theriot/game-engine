#ifndef __ENGINE_IMAGE_HPP
#define __ENGINE_IMAGE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine
{
    class Image
    {
    protected:
        GLuint image;
        std::string filename;

    public:
        Image() = default;
        Image(const char *filename);

        Image &operator=(const Image &other);

        GLuint get() const;
        void use() const;
        const std::string &getFilename() const;
    };

    void to_json(nlohmann::json &j, const Image &image);
    void from_json(const nlohmann::json &j, Image &image);
}

#endif