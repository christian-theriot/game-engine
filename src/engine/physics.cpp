#include <engine/physics.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

Engine::Physics::Plane::Plane(const glm::vec3 &normal, float distance)
    : normal(normal),
      distance(distance)
{
}
float Engine::Physics::Plane::getDistance(const glm::vec3 &point) const
{
    return glm::dot(normal, point) - distance;
}

Engine::Physics::ConvexHull::ConvexHull(const std::vector<glm::vec3> &vertices)
    : vertices(vertices)
{
    computeCentroid();
    computePlanes();
}
void Engine::Physics::ConvexHull::computeCentroid()
{
    centroid = glm::vec3(0.f);
    for (const auto &vertex : vertices)
    {
        centroid += vertex;
    }
    centroid /= static_cast<float>(vertices.size());
}
void Engine::Physics::ConvexHull::computePlanes()
{
    planes.clear();

    // For a convex hull, each face normal points outward
    // This is a simplified version assuming proper mesh structure
    // For production, use proper convex hull algorithms (QuickHull, etc)

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        const glm::vec3 &vertex = vertices[i];
        const glm::vec3 &nextVertex = vertices[(i + 1) % vertices.size()];

        glm::vec3 edge = nextVertex - vertex;
        glm::vec3 toCenter = centroid - vertex;
        glm::vec3 normal = glm::normalize(glm::cross(edge, toCenter));

        // Flip normal if pointing inward
        if (glm::dot(normal, toCenter) < 0)
        {
            normal = -normal;
        }

        planes.emplace_back(normal, glm::dot(normal, vertex));
    }
}
const std::vector<glm::vec3> &Engine::Physics::ConvexHull::getVertices() const
{
    return vertices;
}
const std::vector<Engine::Physics::Plane> &Engine::Physics::ConvexHull::getPlanes() const
{
    return planes;
}
glm::vec3 Engine::Physics::ConvexHull::getCentroid() const
{
    return centroid;
}
bool Engine::Physics::ConvexHull::contains(const glm::vec3 &point) const
{
    for (const auto &plane : planes)
    {
        if (plane.getDistance(point) < 0)
        {
            return false;
        }
    }
    return true;
}
float Engine::Physics::ConvexHull::getDistance(const glm::vec3 &point) const
{
    float minDist = std::numeric_limits<float>::max();

    for (const auto &plane : planes)
    {
        float dist = plane.getDistance(point);
        if (dist < minDist)
        {
            minDist = dist;
        }
    }

    return minDist;
}
glm::vec3 Engine::Physics::ConvexHull::getSupport(const glm::vec3 &direction) const
{
    glm::vec3 support = vertices[0];
    float maxDot = glm::dot(support, direction);

    for (const auto &vertex : vertices)
    {
        float d = glm::dot(vertex, direction);
        if (d > maxDot)
        {
            maxDot = d;
            support = vertex;
        }
    }

    return support;
}
Engine::Physics::ConvexHull Engine::Physics::ConvexHull::createBox(const glm::vec3 &halfExtents)
{
    std::vector<glm::vec3> vertices = {
        glm::vec3(-1, -1, -1) * halfExtents,
        glm::vec3(1, -1, -1) * halfExtents,
        glm::vec3(1, 1, -1) * halfExtents,
        glm::vec3(-1, 1, -1) * halfExtents,
        glm::vec3(-1, -1, 1) * halfExtents,
        glm::vec3(1, -1, 1) * halfExtents,
        glm::vec3(1, 1, 1) * halfExtents,
        glm::vec3(-1, 1, 1) * halfExtents};

    return ConvexHull(vertices);
}
Engine::Physics::ConvexHull Engine::Physics::ConvexHull::createSphere(float radius, int subdivisions)
{
    std::vector<glm::vec3> vertices;

    // Icosphere generation (simplified)
    const float phi = (1.f + std::sqrt(5.f)) / 2.f;
    std::vector<glm::vec3> icosahedron = {
        glm::vec3(-1, phi, 0), glm::vec3(1, phi, 0),
        glm::vec3(-1, -phi, 0), glm::vec3(1, -phi, 0),
        glm::vec3(0, -1, phi), glm::vec3(0, 1, phi),
        glm::vec3(0, -1, -phi), glm::vec3(0, 1, -phi),
        glm::vec3(phi, 0, -1), glm::vec3(phi, 0, 1),
        glm::vec3(-phi, 0, -1), glm::vec3(-phi, 0, 1)};

    for (auto &vertex : icosahedron)
    {
        vertices.push_back(glm::normalize(vertex) * radius);
    }

    return ConvexHull(vertices);
}

Engine::Physics::ConvexHull Engine::Physics::ConvexCollider::getTransformedHull() const
{
    ConvexHull transformed = hull;

    for (auto &vertex : transformed.vertices)
    {
        vertex = rotation * vertex + position;
    }

    transformed.computeCentroid();
    transformed.computePlanes();

    return transformed;
}

Engine::Physics::Rigidbody::Rigidbody(const glm::vec3 &pos, float mass, const ConvexHull &hull)
    : position(pos),
      velocity(0.f),
      acceleration(0.f),
      rotation(1.f),
      angularVelocity(0.f),
      mass(mass),
      inverseMass(1.f / mass),
      friction(0.3f),
      restitution(0.5f),
      isStatic(false),
      isActive(true),
      collider({hull, pos, glm::mat3(1.f)})
{
    computeInertiaTensor();
}
void Engine::Physics::Rigidbody::computeInertiaTensor()
{
    const auto &vertices = collider.hull.getVertices();
    glm::vec3 centroid = collider.hull.getCentroid();

    float maxRadius = 0.f;
    for (const auto &vertex : vertices)
    {
        float radius = glm::distance(vertex, centroid);
        if (radius > maxRadius)
        {
            maxRadius = radius;
        }
    }

    float inertia = (2.f / 5.f) * mass * maxRadius * maxRadius;
    inertiaTensor = glm::mat3(inertia);
    inverseInertiaTensor = glm::mat3(1.f / inertia);
}
glm::vec3 Engine::Physics::Rigidbody::getPosition() const
{
    return position;
}
glm::vec3 Engine::Physics::Rigidbody::getVelocity() const
{
    return velocity;
}
glm::vec3 Engine::Physics::Rigidbody::getAngularVelocity() const
{
    return angularVelocity;
}
float Engine::Physics::Rigidbody::getMass() const
{
    return mass;
}
float Engine::Physics::Rigidbody::getInverseMass() const
{
    return inverseMass;
}
float Engine::Physics::Rigidbody::getRestitution() const
{
    return restitution;
}
float Engine::Physics::Rigidbody::getFriction() const
{
    return friction;
}
Engine::Physics::ConvexCollider &Engine::Physics::Rigidbody::getCollider()
{
    return collider;
}
const Engine::Physics::ConvexCollider &Engine::Physics::Rigidbody::getCollider() const
{
    return collider;
}
bool Engine::Physics::Rigidbody::getIsStatic() const
{
    return isStatic;
}
void Engine::Physics::Rigidbody::setPosition(const glm::vec3 &pos)
{
    position = pos;
    collider.position = pos;
}
void Engine::Physics::Rigidbody::setVelocity(const glm::vec3 &vel)
{
    velocity = vel;
}
void Engine::Physics::Rigidbody::setAngularVelocity(const glm::vec3 &angVel)
{
    angularVelocity = angVel;
}
void Engine::Physics::Rigidbody::setRotation(const glm::mat3 &rot)
{
    rotation = rot;
    collider.rotation = rot;
}
void Engine::Physics::Rigidbody::setRestitution(float rest)
{
    restitution = glm::clamp(rest, 0.f, 1.f);
}
void Engine::Physics::Rigidbody::setFriction(float fric)
{
    friction = glm::clamp(fric, 0.f, 1.f);
}
void Engine::Physics::Rigidbody::setStatic(bool stat)
{
    isStatic = stat;
    inverseMass = isStatic ? 0.f : 1.f / mass;
}
void Engine::Physics::Rigidbody::applyForce(const glm::vec3 &force)
{
    if (isStatic)
    {
        return;
    }

    acceleration += force * inverseMass;
}
void Engine::Physics::Rigidbody::applyImpulse(const glm::vec3 &impulse, const glm::vec3 &contactPoint)
{
    if (isStatic)
    {
        return;
    }

    velocity += impulse * inverseMass;

    glm::vec3 r = contactPoint - position;
    angularVelocity += inverseInertiaTensor * glm::cross(r, impulse);
}
void Engine::Physics::Rigidbody::applyAngularImpulse(const glm::vec3 &impulse)
{
    if (isStatic)
    {
        return;
    }

    angularVelocity += inverseInertiaTensor * impulse;
}
void Engine::Physics::Rigidbody::update(float deltaTime)
{
    if (isStatic || !isActive)
    {
        return;
    }

    velocity += acceleration * deltaTime;
    velocity *= 0.99f;
    angularVelocity *= 0.99f;

    position += velocity * deltaTime;

    glm::vec3 angularDisplacement = angularVelocity * deltaTime;
    float angle = glm::length(angularDisplacement);

    if (angle > 0.001f)
    {
        glm::vec3 axis = glm::normalize(angularDisplacement);
        glm::mat4 rotMat = glm::rotate(glm::mat4(1.f), angle, axis);
        rotation = glm::mat3(rotMat) * rotation;
    }

    collider.position = position;
    collider.rotation = rotation;

    clearForces();
}
void Engine::Physics::Rigidbody::clearForces()
{
    acceleration = glm::vec3(0.f);
}

Engine::Physics::PhysicsSystem::PhysicsSystem(const glm::vec3 &gravity)
    : gravity(gravity),
      solverIterations(4)
{
}
Engine::Physics::Rigidbody *Engine::Physics::PhysicsSystem::addBody(const glm::vec3 &pos, float mass, const ConvexHull &hull)
{
    bodies.push_back(std::make_unique<Rigidbody>(pos, mass, hull));
    return bodies.back().get();
}
void Engine::Physics::PhysicsSystem::removeBody(Rigidbody *body)
{
    auto item = std::find_if(bodies.begin(), bodies.end(), [body](const std::unique_ptr<Rigidbody> &rigidbody)
                             { return rigidbody.get() == body; });

    if (item != bodies.end())
    {
        bodies.erase(item);
    }
}
void Engine::Physics::PhysicsSystem::setGravity(const glm::vec3 &gravity)
{
    this->gravity = gravity;
}
glm::vec3 Engine::Physics::PhysicsSystem::getGravity() const
{
    return gravity;
}
void Engine::Physics::PhysicsSystem::update(float deltaTime)
{
    applyGravity(deltaTime);
    resolveCollisions();

    for (auto &body : bodies)
    {
        body->update(deltaTime);
    }
}
const std::vector<std::unique_ptr<Engine::Physics::Rigidbody>> &Engine::Physics::PhysicsSystem::getBodies() const
{
    return bodies;
}
void Engine::Physics::PhysicsSystem::applyGravity(float deltaTime)
{
    for (auto &body : bodies)
    {
        if (!body->getIsStatic())
        {
            body->applyForce(gravity * body->getMass());
        }
    }
}
void Engine::Physics::PhysicsSystem::resolveCollisions()
{
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        for (size_t j = i + 1; j < bodies.size(); ++j)
        {
            CollisionManifold manifold;
            if (convexConvexCollision(*bodies[i], *bodies[j], manifold))
            {
                resolveCollision(*bodies[i], *bodies[j], manifold);
            }
        }
    }
}
bool Engine::Physics::PhysicsSystem::convexConvexCollision(Rigidbody &bodyA, Rigidbody &bodyB, CollisionManifold &manifold)
{
    // Simplified collision detection using SAT (Separating Axis Theorem)
    const ConvexCollider &colliderA = bodyA.getCollider();
    const ConvexCollider &colliderB = bodyB.getCollider();

    ConvexHull hullA = colliderA.getTransformedHull();
    ConvexHull hullB = colliderB.getTransformedHull();

    float minPenetration = std::numeric_limits<float>::max();
    glm::vec3 minNormal(0);

    // Test all planes from hull A as separating axes
    for (const auto &planeA : hullA.getPlanes())
    {
        float minA = std::numeric_limits<float>::max();
        float maxA = -std::numeric_limits<float>::max();
        float minB = std::numeric_limits<float>::max();
        float maxB = -std::numeric_limits<float>::max();

        // Project all vertices of A onto this axis
        for (const auto &vertexA : hullA.getVertices())
        {
            float proj = glm::dot(vertexA, planeA.normal);
            minA = std::min(minA, proj);
            maxA = std::max(maxA, proj);
        }

        // Project all vertices of B onto this axis
        for (const auto &vertexB : hullB.getVertices())
        {
            float proj = glm::dot(vertexB, planeA.normal);
            minB = std::min(minB, proj);
            maxB = std::max(maxB, proj);
        }

        // Check for gap (separated)
        if (maxA < minB || maxB < minA)
        {
            return false;
        }

        // Calculate penetration depth
        float penetrationDepth;
        if (maxA < maxB)
        {
            penetrationDepth = maxA - minB;
        }
        else
        {
            penetrationDepth = maxB - minA;
        }

        if (std::abs(penetrationDepth) < minPenetration)
        {
            minPenetration = std::abs(penetrationDepth);
            minNormal = planeA.normal;
        }
    }

    // Test all planes from hull B as separating axes
    for (const auto &planeB : hullB.getPlanes())
    {
        float minA = std::numeric_limits<float>::max();
        float maxA = -std::numeric_limits<float>::max();
        float minB = std::numeric_limits<float>::max();
        float maxB = -std::numeric_limits<float>::max();

        // Project all vertices of A onto this axis
        for (const auto &vertexA : hullA.getVertices())
        {
            float proj = glm::dot(vertexA, planeB.normal);
            minA = std::min(minA, proj);
            maxA = std::max(maxA, proj);
        }

        // Project all vertices of B onto this axis
        for (const auto &vertexB : hullB.getVertices())
        {
            float proj = glm::dot(vertexB, planeB.normal);
            minB = std::min(minB, proj);
            maxB = std::max(maxB, proj);
        }

        // Check for gap (separated)
        if (maxA < minB || maxB < minA)
        {
            return false;
        }

        // Calculate penetration depth
        float penetrationDepth;
        if (maxA < maxB)
        {
            penetrationDepth = maxA - minB;
        }
        else
        {
            penetrationDepth = maxB - minA;
        }

        if (std::abs(penetrationDepth) < minPenetration)
        {
            minPenetration = std::abs(penetrationDepth);
            minNormal = planeB.normal;
        }
    }

    // Collision detected
    manifold.contactNormal = minNormal;
    manifold.penetrationDepth = minPenetration;
    manifold.contactPoint = (hullA.getCentroid() + hullB.getCentroid()) * 0.5f;

    return true;
}
void Engine::Physics::PhysicsSystem::resolveCollision(Rigidbody &bodyA, Rigidbody &bodyB, const CollisionManifold &manifold)
{
    // Both static or near-infinite mass
    if (bodyA.getInverseMass() + bodyB.getInverseMass() < 0.0001f)
    {
        return;
    }

    // Ensure normal points from A to B
    glm::vec3 normal = manifold.contactNormal;
    glm::vec3 relativeVel = bodyB.getVelocity() - bodyA.getVelocity();

    // If velocities are separating, no need to resolve
    if (glm::dot(relativeVel, normal) > 0.f)
    {
        return;
    }

    float e = std::min(bodyA.getRestitution(), bodyB.getRestitution());

    float invMassA = bodyA.getInverseMass();
    float invMassB = bodyB.getInverseMass();

    float impulseMagnitude = -(1.f + e) * glm::dot(relativeVel, normal);
    impulseMagnitude /= (invMassA + invMassB);

    glm::vec3 impulse = impulseMagnitude * normal;

    bodyA.applyImpulse(-impulse, manifold.contactPoint);
    bodyB.applyImpulse(impulse, manifold.contactPoint);

    glm::vec3 separation = normal * (manifold.penetrationDepth + 0.01f) * 0.5f;
    bodyA.setPosition(bodyA.getPosition() - separation * invMassA / (invMassA + invMassB));
    bodyB.setPosition(bodyB.getPosition() + separation * invMassB / (invMassA + invMassB));
}