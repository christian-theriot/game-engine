#ifndef __IMAGE_HPP
#define __IMAGE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Image
{
protected:
    GLuint image;

public:
    virtual ~Image() = default;

    GLuint get() const;
    void use() const;
};

class PNG : public Image
{
public:
    PNG(const char *filename);
};

class BMP : public Image
{
public:
    BMP(const char *filename);
};

#endif