/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/image.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Engine::Image::Image(const char *filename)
{
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr)
    {
        std::cout << "Failed to load PNG file: " << filename << std::endl;
        return;
    }

    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);

    switch (channels)
    {
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    default:
        std::cout << "Unsupported number of channels: " << channels << " in file: " << filename << std::endl;
        break;
    }

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

GLuint Engine::Image::get() const
{
    return image;
}

void Engine::Image::use() const
{
    glBindTexture(GL_TEXTURE_2D, image);
}