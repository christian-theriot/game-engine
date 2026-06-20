#ifndef __ENGINE_PHYSICS_HPP
#define __ENGINE_PHYSICS_HPP

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Engine::Physics
{
    struct Plane
    {
        glm::vec3 normal;
        float distance;

        Plane(const glm::vec3 &normal, float distance);

        float getDistance(const glm::vec3 &point) const;
    };

    struct ConvexCollider;

    class ConvexHull
    {
        friend struct ConvexCollider;

        std::vector<glm::vec3> vertices;
        std::vector<Plane> planes;
        glm::vec3 centroid;

        void computeCentroid();
        void computePlanes();

    public:
        ConvexHull() = default;
        ConvexHull(const std::vector<glm::vec3> &vertices);

        const std::vector<glm::vec3> &getVertices() const;
        const std::vector<Plane> &getPlanes() const;
        glm::vec3 getCentroid() const;

        bool contains(const glm::vec3 &point) const;
        float getDistance(const glm::vec3 &point) const;
        glm::vec3 getSupport(const glm::vec3 &direction) const;

        static ConvexHull createBox(const glm::vec3 &halfExtents);
        static ConvexHull createSphere(float radius, int subdivisions = 2);
    };

    struct ConvexCollider
    {
        ConvexHull hull;
        glm::vec3 position;
        glm::mat3 rotation;

        ConvexHull getTransformedHull() const;
    };

    struct CollisionManifold
    {
        glm::vec3 contactPoint;
        glm::vec3 contactNormal;
        float penetrationDepth;

        glm::vec3 bodyAContactPoint;
        glm::vec3 bodyBContactPoint;
    };

    class Rigidbody
    {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::mat3 rotation;
        glm::vec3 angularVelocity;

        float mass;
        float inverseMass;
        glm::mat3 inertiaTensor;
        glm::mat3 inverseInertiaTensor;

        float friction;
        float restitution;

        bool isStatic;
        bool isActive;

        ConvexCollider collider;

        void computeInertiaTensor();

    public:
        Rigidbody(const glm::vec3 &pos, float mass, const ConvexHull &hull);

        glm::vec3 getPosition() const;
        glm::vec3 getVelocity() const;
        glm::vec3 getAngularVelocity() const;
        float getMass() const;
        float getInverseMass() const;
        float getRestitution() const;
        float getFriction() const;
        ConvexCollider &getCollider();
        const ConvexCollider &getCollider() const;
        bool getIsStatic() const;

        void setPosition(const glm::vec3 &pos);
        void setVelocity(const glm::vec3 &velocity);
        void setAngularVelocity(const glm::vec3 &angularVelocity);
        void setRotation(const glm::mat3 &rot);
        void setRestitution(float restitution);
        void setFriction(float friction);
        void setStatic(bool isStatic);

        void applyForce(const glm::vec3 &force);
        void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &contactPoint);
        void applyAngularImpulse(const glm::vec3 &impulse);

        void update(float deltaTime);
        void clearForces();
    };

    class PhysicsSystem
    {
        std::vector<std::unique_ptr<Rigidbody>> bodies;
        glm::vec3 gravity;
        int solverIterations;

        void applyGravity(float deltaTime);
        void resolveCollisions();
        bool convexConvexCollision(Rigidbody &bodyA, Rigidbody &bodyB, CollisionManifold &manifold);
        void resolveCollision(Rigidbody &bodyA, Rigidbody &bodyB, const CollisionManifold &manifold);

    public:
        PhysicsSystem(const glm::vec3 &gravity = glm::vec3(0, -9.81f, 0));

        Rigidbody *addBody(const glm::vec3 &pos, float mass, const ConvexHull &hull);
        void removeBody(Rigidbody *body);

        void setGravity(const glm::vec3 &gravity);
        glm::vec3 getGravity() const;

        void update(float deltaTime);

        const std::vector<std::unique_ptr<Rigidbody>> &getBodies() const;
    };
}

#endif