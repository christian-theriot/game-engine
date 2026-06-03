#include <image.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint Image::get() const
{
    return image;
}

PNG::PNG(const char *filename)
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

BMP::BMP(const char *filename)
{
    std::ifstream file(filename);
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    std::string data = ss.str();
    const unsigned char *bytes = (const unsigned char *)data.c_str();
    std::vector<unsigned char> pixels;

    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;

    memcpy(header, bytes, 54);

    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cout << "Invalid BMP file detected" << std::endl;
        return;
    }

    std::cout << header[0] << header[1] << header[2] << std::endl;

    dataPos = *(int *)&(header[0x0A]);
    imageSize = *(int *)&(header[0x22]);
    width = *(int *)&(header[0x12]);
    height = *(int *)&(header[0x16]);

    if (imageSize == 0)
    {
        imageSize = width * height * 3;
    }
    if (dataPos == 0)
    {
        dataPos = 54;
    }

    pixels.resize(imageSize);
    memcpy(&pixels[0], &bytes[dataPos], imageSize);

    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels.data());

    std::cout << imageSize << " " << width << "x" << height << std::endl;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Image::use() const
{
    glBindTexture(GL_TEXTURE_2D, image);
}