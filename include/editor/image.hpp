#ifndef __IMAGE_HPP
#define __IMAGE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Image
{
protected:
    GLuint image;

public:
    Image(const char *filename);

    GLuint get() const;
    void use() const;
};

#endif