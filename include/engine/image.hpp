#ifndef __ENGINE_IMAGE_HPP
#define __ENGINE_IMAGE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class Image
    {
    protected:
        GLuint image;

    public:
        Image(const char *filename);

        GLuint get() const;
        void use() const;
    };
}

#endif