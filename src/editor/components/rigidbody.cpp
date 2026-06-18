#include <editor/components/rigidbody.hpp>

Editor::Components::RigidbodyComponent::RigidbodyComponent(std::unique_ptr<Editor::Physics::Rigidbody> body)
    : rigidbody(std::move(body))
{
}
Editor::Physics::Rigidbody *Editor::Components::RigidbodyComponent::getRigidbody() const
{
    return rigidbody.get();
}
void Editor::Components::RigidbodyComponent::setRigidbody(std::unique_ptr<Editor::Physics::Rigidbody> body)
{
    rigidbody = std::move(body);
}