#ifndef __ENGINE_CAMERA_HPP
#define __ENGINE_CAMERA_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <engine/v1/mesh.hpp>
#include <engine/v1/renderable.hpp>
#include <engine/v1/transform.hpp>

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