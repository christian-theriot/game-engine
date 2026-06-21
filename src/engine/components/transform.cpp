/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/components/transform.hpp>

Engine::Components::TransformComponent::TransformComponent(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale)
    : transform(std::make_unique<Transform>())
{
    transform->setPosition(pos);
    transform->setRotation(rot);
    transform->setScale(scale);
}
Engine::Transform *Engine::Components::TransformComponent::getTransform() const
{
    return transform.get();
}
void Engine::Components::TransformComponent::setTransform(std::unique_ptr<Transform> t)
{
    transform = std::move(t);
}