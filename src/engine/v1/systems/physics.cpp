/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/systems/physics.hpp>
#include <engine/v1/world.hpp>
#include <engine/v1/components/rigidbody.hpp>
#include <engine/v1/components/transform.hpp>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <cstdarg>
#include <iostream>
#include <thread>

static void TraceImpl(const char *inFmt, ...)
{
    va_list list;
    va_start(list, inFmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFmt, list);
    va_end(list);
    std::cout << buffer << std::endl;
}

Engine::Systems::BPLayerInterfaceImpl::BPLayerInterfaceImpl()
{
    objectToBroadPhase[Layers::STATIC] = BroadPhaseLayers::STATIC;
    objectToBroadPhase[Layers::DYNAMIC] = BroadPhaseLayers::DYNAMIC;
}
JPH::uint Engine::Systems::BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const
{
    return BroadPhaseLayers::NUM_LAYERS;
}
JPH::BroadPhaseLayer Engine::Systems::BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
    return objectToBroadPhase[inLayer];
}
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char *Engine::Systems::BPLayerInterfaceImpl::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
{
    switch (inLayer)
    {
    case BroadPhaseLayers::STATIC:
        return "STATIC";
    case BroadPhaseLayers::DYNAMIC:
        return "DYNAMIC";
    default:
        return "UNKNOWN";
    }
}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
bool Engine::Systems::ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
{
    switch (inLayer1)
    {
    case Layers::STATIC:
        return inLayer2 == BroadPhaseLayers::DYNAMIC;
    case Layers::DYNAMIC:
        return true;
    default:
        return false;
    }
}

bool Engine::Systems::ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const
{
    switch (inLayer1)
    {
    case Layers::STATIC:
        return inLayer2 == Layers::DYNAMIC;
    case Layers::DYNAMIC:
        return true;
    default:
        return false;
    }
}

Engine::Systems::PhysicsSystem::PhysicsSystem()
{
    JPH::RegisterDefaultAllocator();
    JPH::Trace = TraceImpl;

    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    physicsSystem = std::make_unique<JPH::PhysicsSystem>();

    physicsSystem->Init(cMaxBodies, cMaxBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
    physicsSystem->SetGravity(JPH::Vec3(0, -9.81f, 0));
}
Engine::Systems::PhysicsSystem::~PhysicsSystem()
{
    JPH::BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
    for (const auto &pair : bodies)
    {
        bodyInterface.RemoveBody(pair.second);
        bodyInterface.DestroyBody(pair.second);
    }

    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}
void Engine::Systems::PhysicsSystem::update(World *world, float deltaTime)
{
    const float maxStep = 1.f / 60.f;
    deltaTime = std::min(deltaTime, maxStep);

    const int collisionSteps = 8;
    physicsSystem->Update(deltaTime, collisionSteps, tempAllocator.get(), jobSystem.get());

    auto entities = world->getEntitiesWithComponent<Engine::Components::RigidbodyComponent>();
    for (auto *entity : entities)
    {
        auto *rigidbody = entity->getComponent<Engine::Components::RigidbodyComponent>();
        auto *transform = entity->getComponent<Engine::Components::TransformComponent>();

        if (rigidbody && transform && !rigidbody->getIsStatic())
        {
            uint32_t handle = rigidbody->getHandle();

            transform->getTransform()->setPosition(getPosition(handle));
            transform->getTransform()->setRotation(getRotation(handle));
        }
    }
}
uint32_t Engine::Systems::PhysicsSystem::createBox(const glm::vec3 &position, const glm::vec3 &halfExtents, float mass, bool isStatic)
{
    JPH::BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();

    JPH::BoxShapeSettings shapeSettings(JPH::Vec3(halfExtents.x, halfExtents.y, halfExtents.z));
    shapeSettings.SetEmbedded();
    JPH::ShapeRefC shape = shapeSettings.Create().Get();

    JPH::EMotionType motionType = isStatic ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic;
    JPH::ObjectLayer layer = isStatic ? Layers::STATIC : Layers::DYNAMIC;

    JPH::BodyCreationSettings bodySettings(shape, JPH::RVec3(position.x, position.y, position.z), JPH::Quat::sIdentity(), motionType, layer);
    if (!isStatic)
    {
        bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
        bodySettings.mMassPropertiesOverride.mMass = mass;
        bodySettings.mMotionQuality = JPH::EMotionQuality::LinearCast;
        bodySettings.mAllowSleeping = false;
    }

    JPH::Body *body = bodyInterface.CreateBody(bodySettings);
    bodyInterface.AddBody(body->GetID(), JPH::EActivation::Activate);

    uint32_t handle = nextHandle++;
    bodies[handle] = body->GetID();
    return handle;
}
void Engine::Systems::PhysicsSystem::destroyBody(uint32_t handle)
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return;

    JPH::BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
    bodyInterface.RemoveBody(it->second);
    bodyInterface.DestroyBody(it->second);
    bodies.erase(it);
}
glm::vec3 Engine::Systems::PhysicsSystem::getPosition(uint32_t handle) const
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return glm::vec3(0.f);

    JPH::RVec3 pos = physicsSystem->GetBodyInterface().GetPosition(it->second);
    return glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());
}
glm::quat Engine::Systems::PhysicsSystem::getRotation(uint32_t handle) const
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return glm::quat(1.f, 0.f, 0.f, 0.f);

    JPH::Quat rot = physicsSystem->GetBodyInterface().GetRotation(it->second);
    return glm::quat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
}
void Engine::Systems::PhysicsSystem::setRestitution(uint32_t handle, float restitution)
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return;

    physicsSystem->GetBodyInterface().SetRestitution(it->second, restitution);
}
void Engine::Systems::PhysicsSystem::setFriction(uint32_t handle, float friction)
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return;

    physicsSystem->GetBodyInterface().SetFriction(it->second, friction);
}
void Engine::Systems::PhysicsSystem::setAngularDamping(uint32_t handle, float angularDamping)
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return;

    JPH::BodyLockWrite lock(physicsSystem->GetBodyLockInterface(), it->second);
    if (lock.Succeeded())
    {
        lock.GetBody().GetMotionProperties()->SetAngularDamping(angularDamping);
    }
}
void Engine::Systems::PhysicsSystem::applyForce(uint32_t handle, const glm::vec3 &force)
{
    auto it = bodies.find(handle);
    if (it == bodies.end())
        return;

    JPH::BodyLockWrite lock(physicsSystem->GetBodyLockInterface(), it->second);
    if (lock.Succeeded())
    {
        lock.GetBody().AddForce(JPH::Vec3(force.x, force.y, force.z));
    }
}