#include <editor/components/transform.hpp>

Editor::Components::TransformComponent::TransformComponent(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale)
    : transform(std::make_unique<Transform>())
{
    transform->setPosition(pos);
    transform->setRotation(rot);
    transform->setScale(scale);
}
Editor::Transform *Editor::Components::TransformComponent::getTransform() const
{
    return transform.get();
}
void Editor::Components::TransformComponent::setTransform(std::unique_ptr<Transform> t)
{
    transform = std::move(t);
}