#include <editor/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Editor::Transform::updateMatrix()
{
    if (!isDirty)
        return;

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.f, 0.f, 0.f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(0.f, 1.f, 0.f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(0.f, 0.f, 1.f));

    localMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    isDirty = false;
}
Editor::Transform::Transform()
    : position(0.f), rotation(0.f), scale(1.f), localMatrix(1.f), worldMatrix(1.f), isDirty(true)
{
}
void Editor::Transform::setPosition(const glm::vec3 &pos)
{
    position = pos;
    isDirty = true;
}
void Editor::Transform::setRotation(const glm::vec3 &rot)
{
    rotation = rot;
    isDirty = true;
}
void Editor::Transform::setScale(const glm::vec3 &newScale)
{
    scale = newScale;
    isDirty = true;
}
glm::vec3 Editor::Transform::getPosition() const
{
    return position;
}
glm::vec3 Editor::Transform::getRotation() const
{
    return rotation;
}
glm::vec3 Editor::Transform::getScale() const
{
    return scale;
}
void Editor::Transform::update(const glm::mat4 &parentMatrix)
{
    updateMatrix();

    worldMatrix = parentMatrix * localMatrix;
}
glm::vec3 Editor::Transform::getWorldPosition() const
{
    return glm::vec3(worldMatrix[3]);
}

glm::mat4 Editor::Transform::getLocalMatrix() const
{
    return localMatrix;
}
glm::mat4 Editor::Transform::getWorldMatrix() const
{
    return worldMatrix;
}