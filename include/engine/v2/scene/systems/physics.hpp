#ifndef __ENGINE_V2_SCENE_SYSTEMS_PHYSICS_HPP
#define __ENGINE_V2_SCENE_SYSTEMS_PHYSICS_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/scene/system.hpp>
#include <engine/v2/scene/components/rigidbody.hpp>
#include <engine/v2/scene/components/transform.hpp>
#include <engine/v2/scene/entity.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <unordered_map>

namespace Engine::Scene::Systems
{
    namespace Layers
    {
        static constexpr JPH::ObjectLayer STATIC = 0;
        static constexpr JPH::ObjectLayer DYNAMIC = 1;
        static constexpr JPH::ObjectLayer TRIGGER = 2;
        static constexpr JPH::ObjectLayer NUM_LAYERS = 3;
    }

    namespace BroadPhaseLayers
    {
        static constexpr JPH::BroadPhaseLayer STATIC(0);
        static constexpr JPH::BroadPhaseLayer DYNAMIC(1);
        static constexpr JPH::BroadPhaseLayer TRIGGER(2);
        static constexpr JPH::uint NUM_LAYERS(3);
    }

    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
    {
        JPH::BroadPhaseLayer objectToBroadPhase[Layers::NUM_LAYERS];

    public:
        BPLayerInterfaceImpl();

        JPH::uint GetNumBroadPhaseLayers() const override;
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
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

    class Physics final : public System
    {
        std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
        std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
        std::unique_ptr<JPH::PhysicsSystem> physicsSystem;

        BPLayerInterfaceImpl broadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
        ObjectLayerPairFilterImpl objectLayerPairFilter;

        std::unordered_map<uint32_t, JPH::BodyID> entityToBodyMap;
        uint32_t nextHandle = 1;

        static constexpr JPH::uint MAX_BODIES = 1024;
        static constexpr JPH::uint MAX_BODY_MUTEXES = 0;
        static constexpr JPH::uint MAX_BODY_PAIRS = 1024;
        static constexpr JPH::uint MAX_CONTACT_CONSTRAINTS = 1024;

    public:
        Physics();
        ~Physics();

        inline uint32_t add(Scene::Entity *entity)
        {
            if (auto *rigidbody = entity->getComponent<Scene::Components::Rigidbody>())
            {
                uint32_t handle = createBox(entity->getComponent<Scene::Components::Transform>()->getTransform().position, rigidbody->getHalfExtents(), rigidbody->getMass(), rigidbody->getIsStatic());
                rigidbody->setHandle(handle);
                setRestitution(handle, rigidbody->getRestitution());
                setFriction(handle, rigidbody->getFriction());
                return handle;
            }

            return 0;
        }
        void update(Scene::World *world, float deltaTime) override;

        uint32_t createBox(const glm::vec3 &position, const glm::vec3 &halfExtents, float mass, bool isStatic);
        void destroyBody(uint32_t handle);

        glm::vec3 getPosition(uint32_t handle) const;
        glm::quat getRotation(uint32_t handle) const;

        Physics &setPosition(uint32_t handle, const glm::vec3 &position);
        Physics &setRotation(uint32_t handle, const glm::quat &rotation);
        Physics &setRestitution(uint32_t handle, float restitution);
        Physics &setFriction(uint32_t handle, float friction);
        Physics &setAngularDamping(uint32_t handle, float angularDamping);
        Physics &applyForce(uint32_t handle, const glm::vec3 &force);
    };
}

#endif