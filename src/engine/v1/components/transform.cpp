/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/components/transform.hpp>

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

void Engine::Components::to_json(nlohmann::json &j, const TransformComponent &tc)
{
    j["transform"] = *tc.getTransform();
}
void Engine::Components::from_json(const nlohmann::json &j, TransformComponent &tc)
{
    if (j.contains("transform"))
    {
        tc.setTransform(std::make_unique<Transform>(j.at("transform").get<Transform>()));
    }
}