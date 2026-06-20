#include <engine/components/rigidbody.hpp>

Engine::Components::RigidbodyComponent::RigidbodyComponent(std::unique_ptr<Engine::Physics::Rigidbody> body)
    : rigidbody(std::move(body))
{
}
Engine::Physics::Rigidbody *Engine::Components::RigidbodyComponent::getRigidbody() const
{
    return rigidbody.get();
}
void Engine::Components::RigidbodyComponent::setRigidbody(std::unique_ptr<Engine::Physics::Rigidbody> body)
{
    rigidbody = std::move(body);
}