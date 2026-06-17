#ifndef __EDITOR_CAMERA_HPP
#define __EDITOR_CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <editor/mesh.hpp>
#include <editor/renderable.hpp>
#include <editor/transform.hpp>

namespace Editor
{
    class Camera
    {
        glm::vec3 position;
        glm::mat4 projection;
        glm::mat4 view;

    public:
        Camera(GLfloat fov = 45.0f, GLfloat width = 1024, GLfloat height = 768, GLfloat min = 0.01f, GLfloat max = 100.0f);

        void rotate(glm::vec3 axis);
        void render(const Renderable &mesh, const Transform &transform) const;
    };
}

#endif