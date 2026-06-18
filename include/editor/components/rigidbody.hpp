#ifndef __EDITOR_COMPONENTS_RIGIDBODY_HPP
#define __EDITOR_COMPONENTS_RIGIDBODY_HPP

#include <editor/component.hpp>
#include <editor/physics.hpp>

namespace Editor::Components
{
    class RigidbodyComponent : public Component
    {
        std::unique_ptr<Editor::Physics::Rigidbody> rigidbody;

    public:
        RigidbodyComponent(std::unique_ptr<Editor::Physics::Rigidbody> body);
        Editor::Physics::Rigidbody *getRigidbody() const;
        void setRigidbody(std::unique_ptr<Editor::Physics::Rigidbody> body);
    };
}

#endif