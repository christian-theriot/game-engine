/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/components/rigidbody.hpp>
Engine::Components::RigidbodyComponent::RigidbodyComponent(const glm::vec3 &halfExtents, float mass, bool isStatic)
    : handle(0),
      halfExtents(halfExtents),
      mass(mass),
      isStatic(isStatic)
{
}
void Engine::Components::RigidbodyComponent::setHandle(uint32_t handle)
{
    this->handle = handle;
}
uint32_t Engine::Components::RigidbodyComponent::getHandle() const
{
    return handle;
}
const glm::vec3 &Engine::Components::RigidbodyComponent::getHalfExtents() const
{
    return halfExtents;
}
float Engine::Components::RigidbodyComponent::getMass() const
{
    return mass;
}
bool Engine::Components::RigidbodyComponent::getIsStatic() const
{
    return isStatic;
}