#ifndef __MATERIAL_HPP
#define __MATERIAL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

#endif