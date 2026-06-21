#ifndef __ENGINE_MATERIAL_HPP
#define __ENGINE_MATERIAL_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class Material
    {
        GLuint program;

    public:
        Material();
        Material(const Material &material);
        Material(const char *filename);

        Material &operator=(const char *filename);

        GLuint get() const;
    };
}

#endif