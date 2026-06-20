#ifndef __ENGINE_SYSTEMS_PHYSICS_HPP
#define __ENGINE_SYSTEMS_PHYSICS_HPP

#include <engine/system.hpp>
#include <engine/physics.hpp>

namespace Engine::Systems
{
    class PhysicsSystem : public System
    {
        std::unique_ptr<Engine::Physics::PhysicsSystem> physics;

    public:
        PhysicsSystem();
        void update(World *world, float deltaTime) override;
        Engine::Physics::PhysicsSystem *getPhysics() const;
    };
}

#endif