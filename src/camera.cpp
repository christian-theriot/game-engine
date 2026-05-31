#include <camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat min, GLfloat max)
    : projection(glm::perspective(glm::radians(fov), width / height, min, max)),
      view(glm::lookAt(glm::vec3(0, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)))
{
}

void Camera::render(const Mesh &mesh) const
{
    mesh.render(projection * view);
}