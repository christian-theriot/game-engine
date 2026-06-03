#ifndef __EDITOR_MATERIAL_HPP
#define __EDITOR_MATERIAL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Editor
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