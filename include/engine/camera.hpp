#ifndef __ENGINE_CAMERA_HPP
#define __ENGINE_CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <engine/mesh.hpp>
#include <engine/renderable.hpp>
#include <engine/transform.hpp>

namespace Engine
{
    class Camera
    {
        glm::vec3 origin;
        glm::vec3 position;
        glm::mat4 projection;
        glm::mat4 view;

    public:
        Camera(GLfloat fov = 45.0f, GLfloat width = 1024, GLfloat height = 768, GLfloat min = 0.01f, GLfloat max = 100.0f);

        glm::mat4 getViewProjection() const;

        void translate(glm::vec3 position);
        void rotate(GLfloat radius, glm::vec2 angles);
        void rotate(glm::vec3 axis);
    };
}

#endif