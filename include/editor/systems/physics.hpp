#ifndef __EDITOR_SYSTEMS_PHYSICS_HPP
#define __EDITOR_SYSTEMS_PHYSICS_HPP

#include <editor/system.hpp>
#include <editor/physics.hpp>

namespace Editor::Systems
{
    class PhysicsSystem : public System
    {
        std::unique_ptr<Editor::Physics::PhysicsSystem> physics;

    public:
        PhysicsSystem();
        void update(World *world, float deltaTime) override;
        Editor::Physics::PhysicsSystem *getPhysics() const;
    };
}

#endif