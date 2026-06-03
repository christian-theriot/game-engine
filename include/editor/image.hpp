#ifndef __EDITOR_IMAGE_HPP
#define __EDITOR_IMAGE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Editor
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