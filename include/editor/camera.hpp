#ifndef __CAMERA_HPP
#define __CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <editor/mesh.hpp>

class Camera
{
    glm::vec3 position;
    glm::mat4 projection;
    glm::mat4 view;

public:
    Camera(GLfloat fov = 45.0f, GLfloat width = 1024, GLfloat height = 768, GLfloat min = 0.01f, GLfloat max = 100.0f);

    void rotate(glm::vec3 axis);
    void render(const Mesh &mesh) const;
};

#endif