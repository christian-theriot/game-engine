#ifndef __ENGINE_COMPONENTS_RIGIDBODY_HPP
#define __ENGINE_COMPONENTS_RIGIDBODY_HPP

#include <engine/component.hpp>
#include <engine/physics.hpp>

namespace Engine::Components
{
    class RigidbodyComponent : public Component
    {
        std::unique_ptr<Engine::Physics::Rigidbody> rigidbody;

    public:
        RigidbodyComponent(std::unique_ptr<Engine::Physics::Rigidbody> body);
        Engine::Physics::Rigidbody *getRigidbody() const;
        void setRigidbody(std::unique_ptr<Engine::Physics::Rigidbody> body);
    };
}

#endif