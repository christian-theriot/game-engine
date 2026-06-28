#include <engine/scene/systems/physics.hpp>
#include <engine/scene/world.hpp>
#include <engine/scene/components/rigidbody.hpp>
#include <engine/scene/components/transform.hpp>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>

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

Engine::Scene::Systems::BPLayerInterfaceImpl::BPLayerInterfaceImpl()
{
    objectToBroadPhase[Layers::STATIC] = BroadPhaseLayers::STATIC;
    objectToBroadPhase[Layers::DYNAMIC] = BroadPhaseLayers::DYNAMIC;
    objectToBroadPhase[Layers::TRIGGER] = BroadPhaseLayers::TRIGGER;
}
JPH::uint Engine::Scene::Systems::BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const
{
    return BroadPhaseLayers::NUM_LAYERS;
}
JPH::BroadPhaseLayer Engine::Scene::Systems::BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
    return objectToBroadPhase[inLayer];
}
bool Engine::Scene::Systems::ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
{
    switch (inLayer1)
    {
    case Layers::STATIC:
    case Layers::TRIGGER:
        return inLayer2 == BroadPhaseLayers::DYNAMIC;
    case Layers::DYNAMIC:
        return true;
    default:
        return false;
    }
}

bool Engine::Scene::Systems::ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::ObjectLayer inLayer2) const
{
    switch (inLayer1)
    {
    case Layers::STATIC:
    case Layers::TRIGGER:
        return inLayer2 == Layers::DYNAMIC;
    case Layers::DYNAMIC:
        return true;
    default:
        return false;
    }
}

Engine::Scene::Systems::Physics::Physics()
{
    JPH::RegisterDefaultAllocator();
    JPH::Trace = TraceImpl;

    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    physicsSystem = std::make_unique<JPH::PhysicsSystem>();

    physicsSystem->Init(MAX_BODIES, MAX_BODY_MUTEXES, MAX_BODY_PAIRS, MAX_CONTACT_CONSTRAINTS, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
    physicsSystem->SetGravity(JPH::Vec3(0, -9.81f, 0));
}
Engine::Scene::Systems::Physics::~Physics()
{
    JPH::BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
    for (const auto &pair : entityToBodyMap)
    {
        bodyInterface.RemoveBody(pair.second);
        bodyInterface.DestroyBody(pair.second);
    }
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}
void Engine::Scene::Systems::Physics::update(Scene::World *world, float deltaTime)
{
    const float maxStep = 1.f / 60.f;
    deltaTime = std::min(deltaTime, maxStep);

    const int collisionSteps = 8;
    physicsSystem->Update(deltaTime, collisionSteps, tempAllocator.get(), jobSystem.get());

    auto entities = world->getEntitiesWithComponent<Components::Rigidbody>();
    for (auto &entity : entities)
    {
        auto *rigidbody = entity->getComponent<Components::Rigidbody>();
        auto *transform = entity->getComponent<Components::Transform>();

        if (rigidbody && transform && !rigidbody->getIsStatic())
        {
            uint32_t handle = rigidbody->getHandle();

            transform->getTransform().position = getPosition(handle);
            transform->getTransform().rotation = getRotation(handle);
        }
    }
}
uint32_t Engine::Scene::Systems::Physics::createBox(const glm::vec3 &position, const glm::vec3 &halfExtents, float mass, bool isStatic)
{
    JPH::BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();

    JPH::BoxShapeSettings shapeSettings(JPH::Vec3(halfExtents.x, halfExtents.y, halfExtents.z));
    shapeSettings.SetEmbedded();
    JPH::ShapeRefC shape = shapeSettings.Create().Get();

    JPH::EMotionType motionType = isStatic ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic;
    JPH::ObjectLayer objectLayer = isStatic ? Layers::STATIC : Layers::DYNAMIC;

    JPH::BodyCreationSettings bodySettings(shape, JPH::RVec3(position.x, position.y, position.z), JPH::Quat::sIdentity(), motionType, objectLayer);
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
    entityToBodyMap[handle] = body->GetID();
    return handle;
}
void Engine::Scene::Systems::Physics::destroyBody(uint32_t handle)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return;

    JPH::BodyInterface &bodyInterface = physicsSystem->GetBodyInterface();
    bodyInterface.RemoveBody(it->second);
    bodyInterface.DestroyBody(it->second);
    entityToBodyMap.erase(it);
}
glm::vec3 Engine::Scene::Systems::Physics::getPosition(uint32_t handle) const
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return glm::vec3(0.f);

    JPH::RVec3 pos = physicsSystem->GetBodyInterface().GetPosition(it->second);
    return glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());
}
glm::quat Engine::Scene::Systems::Physics::getRotation(uint32_t handle) const
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return glm::quat(1.f, 0.f, 0.f, 0.f);

    JPH::Quat rot = physicsSystem->GetBodyInterface().GetRotation(it->second);
    return glm::quat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
}
Engine::Scene::Systems::Physics &Engine::Scene::Systems::Physics::setPosition(uint32_t handle, const glm::vec3 &position)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return *this;

    JPH::RVec3 pos(position.x, position.y, position.z);
    physicsSystem->GetBodyInterface().SetPosition(it->second, pos, JPH::EActivation::DontActivate);
    return *this;
}
Engine::Scene::Systems::Physics &Engine::Scene::Systems::Physics::setRotation(uint32_t handle, const glm::quat &rotation)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return *this;

    JPH::Quat rot(rotation.w, rotation.x, rotation.y, rotation.z);
    physicsSystem->GetBodyInterface().SetRotation(it->second, rot, JPH::EActivation::DontActivate);
    return *this;
}
Engine::Scene::Systems::Physics &Engine::Scene::Systems::Physics::setRestitution(uint32_t handle, float restitution)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return *this;

    physicsSystem->GetBodyInterface().SetRestitution(it->second, restitution);
    return *this;
}
Engine::Scene::Systems::Physics &Engine::Scene::Systems::Physics::setFriction(uint32_t handle, float friction)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return *this;

    physicsSystem->GetBodyInterface().SetFriction(it->second, friction);
    return *this;
}
Engine::Scene::Systems::Physics &Engine::Scene::Systems::Physics::setAngularDamping(uint32_t handle, float angularDamping)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return *this;

    JPH::BodyLockWrite lock(physicsSystem->GetBodyLockInterface(), it->second);
    if (lock.Succeeded())
    {
        lock.GetBody().GetMotionProperties()->SetAngularDamping(angularDamping);
    }
    return *this;
}
Engine::Scene::Systems::Physics &Engine::Scene::Systems::Physics::applyForce(uint32_t handle, const glm::vec3 &force)
{
    auto it = entityToBodyMap.find(handle);
    if (it == entityToBodyMap.end())
        return *this;

    JPH::BodyLockWrite lock(physicsSystem->GetBodyLockInterface(), it->second);
    if (lock.Succeeded())
    {
        lock.GetBody().AddForce(JPH::Vec3(force.x, force.y, force.z));
    }
    return *this;
}