#ifndef __ENGINE_SCENE_COMPONENTS_RIGIDBODY_HPP
#define __ENGINE_SCENE_COMPONENTS_RIGIDBODY_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/component.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace Engine::Scene::Components
{
    class Rigidbody : public Component
    {
        uint32_t handle;
        glm::vec3 halfExtents;
        float mass;
        bool isStatic;
        float restitution;
        float friction;

    public:
        inline Rigidbody(const glm::vec3 &halfExtents = glm::vec3(0.5f), float mass = 1.f, bool isStatic = false, float restitution = 0.05f, float friction = 0.5f)
            : halfExtents(halfExtents), mass(mass), isStatic(isStatic), restitution(restitution), friction(friction) {}

        inline void setHandle(uint32_t handle) { this->handle = handle; }
        inline uint32_t getHandle() const { return handle; }
        inline const glm::vec3 &getHalfExtents() const { return halfExtents; }
        inline void setHalfExtents(const glm::vec3 &halfExtents) { this->halfExtents = halfExtents; }
        inline void setMass(float mass) { this->mass = mass; }
        inline void setIsStatic(bool isStatic) { this->isStatic = isStatic; }
        inline void setRestitution(float restitution) { this->restitution = restitution; }
        inline void setFriction(float friction) { this->friction = friction; }
        inline float getMass() const { return mass; }
        inline bool getIsStatic() const { return isStatic; }
        inline float getRestitution() const { return restitution; }
        inline float getFriction() const { return friction; }
        inline std::string type() const override { return "Rigidbody"; }
    };

    void to_json(nlohmann::json &j, const Rigidbody &rigidbody);
    void from_json(const nlohmann::json &j, Rigidbody &rigidbody);
}

#endif