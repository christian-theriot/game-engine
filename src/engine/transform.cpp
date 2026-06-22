/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Engine::Transform::updateMatrix()
{
    if (!isDirty)
        return;

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), position);
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);

    rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1.f, 0.f, 0.f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.f, 1.f, 0.f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.f, 0.f, 1.f));

    localMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    isDirty = false;
}
Engine::Transform::Transform()
    : position(0.f), rotation(0.f), scale(1.f), localMatrix(1.f), worldMatrix(1.f), isDirty(true)
{
}
void Engine::Transform::setPosition(const glm::vec3 &pos)
{
    position = pos;
    isDirty = true;
}
void Engine::Transform::setRotation(const glm::vec3 &rot)
{
    rotation = rot;
    isDirty = true;
}
void Engine::Transform::setRotation(const glm::quat &rot)
{
    rotation = glm::eulerAngles(rot);
    isDirty = true;
}
void Engine::Transform::setScale(const glm::vec3 &newScale)
{
    scale = newScale;
    isDirty = true;
}
glm::vec3 Engine::Transform::getPosition() const
{
    return position;
}
glm::vec3 Engine::Transform::getRotation() const
{
    return rotation;
}
glm::vec3 Engine::Transform::getScale() const
{
    return scale;
}
void Engine::Transform::update(const glm::mat4 &parentMatrix)
{
    updateMatrix();

    worldMatrix = parentMatrix * localMatrix;
}
glm::vec3 Engine::Transform::getWorldPosition() const
{
    return glm::vec3(worldMatrix[3]);
}

glm::mat4 Engine::Transform::getLocalMatrix() const
{
    return localMatrix;
}
glm::mat4 Engine::Transform::getWorldMatrix() const
{
    return worldMatrix;
}

void Engine::to_json(nlohmann::json &j, const Transform &t)
{
    j["position"] = t.getPosition();
    j["rotation"] = t.getRotation();
    j["scale"] = t.getScale();
}
void Engine::from_json(const nlohmann::json &j, Transform &t)
{
    t.setPosition(j.at("position").get<glm::vec3>());
    t.setRotation(j.at("rotation").get<glm::vec3>());
    t.setScale(j.at("scale").get<glm::vec3>());
}