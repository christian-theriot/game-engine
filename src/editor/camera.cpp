#include <editor/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

Editor::Camera::Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat min, GLfloat max)
    : position(glm::vec3(4, 3, 3)),
      projection(glm::perspective(glm::radians(fov), width / height, min, max)),
      view(glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)))
{
}

void Editor::Camera::rotate(glm::vec3 axis)
{
    position = axis;
    view = glm::lookAt(position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
glm::mat4 Editor::Camera::getViewProjection() const
{
    return projection * view;
}