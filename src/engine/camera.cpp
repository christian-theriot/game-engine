#include <engine/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

Engine::Camera::Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat min, GLfloat max)
    : position(glm::vec3(4, 3, 3)),
      origin(glm::vec3(0, 0, 0)),
      projection(glm::perspective(glm::radians(fov), width / height, min, max)),
      view(glm::lookAt(position, origin, glm::vec3(0, 1, 0)))
{
}
void Engine::Camera::translate(glm::vec3 position)
{
    this->position += position;
    view = glm::lookAt(this->position, origin, glm::vec3(0, 1, 0));
}
void Engine::Camera::rotate(GLfloat radius, glm::vec2 angles)
{
    position.x = radius * sin(angles.y) * cos(angles.x);
    position.y = radius * cos(angles.y);
    position.z = radius * sin(angles.y) * sin(angles.x);

    view = glm::lookAt(position, origin, glm::vec3(0, 1, 0));
}
void Engine::Camera::rotate(glm::vec3 axis)
{
    position = axis;
    view = glm::lookAt(position, origin, glm::vec3(0, 1, 0));
}
glm::mat4 Engine::Camera::getViewProjection() const
{
    return projection * view;
}