#ifndef __ENGINE_SYSTEMS_PHYSICS_HPP
#define __ENGINE_SYSTEMS_PHYSICS_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/system.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <unordered_map>
#include <memory>

namespace Engine::Systems
{
    namespace Layers
    {
        static constexpr JPH::ObjectLayer STATIC = 0;
        static constexpr JPH::ObjectLayer DYNAMIC = 1;
        static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    }

    namespace BroadPhaseLayers
    {
        static constexpr JPH::BroadPhaseLayer STATIC(0);
        static constexpr JPH::BroadPhaseLayer DYNAMIC(1);
        static constexpr JPH::uint NUM_LAYERS(2);
    }

    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
    {
        JPH::BroadPhaseLayer objectToBroadPhase[Layers::NUM_LAYERS];

    public:
        BPLayerInterfaceImpl();

        JPH::uint GetNumBroadPhaseLayers() const override;
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        const char *GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
    };

    class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
    };

    class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
    {
    public:
        bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const override;
    };

    class PhysicsSystem : public System
    {
        std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
        std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
        std::unique_ptr<JPH::PhysicsSystem> physicsSystem;

        BPLayerInterfaceImpl broadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
        ObjectLayerPairFilterImpl objectLayerPairFilter;

        std::unordered_map<uint32_t, JPH::BodyID> bodies;
        uint32_t nextHandle = 1;

        static constexpr JPH::uint cMaxBodies = 1024;
        static constexpr JPH::uint cMaxBodyMutexes = 0;
        static constexpr JPH::uint cMaxBodyPairs = 1024;
        static constexpr JPH::uint cMaxContactConstraints = 1024;

    public:
        PhysicsSystem();
        ~PhysicsSystem();

        void update(World *world, float deltaTime) override;

        uint32_t createBox(const glm::vec3 &position, const glm::vec3 &halfExtents, float mass, bool isStatic);
        void destroyBody(uint32_t handle);

        glm::vec3 getPosition(uint32_t handle) const;
        glm::quat getRotation(uint32_t handle) const;

        void setRestitution(uint32_t handle, float restitution);
        void setFriction(uint32_t handle, float friction);
        void setAngularDamping(uint32_t handle, float angularDamping);
        void applyForce(uint32_t handle, const glm::vec3 &force);
    };
}

#endif