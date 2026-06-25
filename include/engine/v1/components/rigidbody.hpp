#ifndef __ENGINE_COMPONENTS_RIGIDBODY_HPP
#define __ENGINE_COMPONENTS_RIGIDBODY_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/component.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Engine::Components
{
    class RigidbodyComponent : public Component
    {
        uint32_t handle;
        glm::vec3 halfExtents;
        float mass;
        bool isStatic;

    public:
        RigidbodyComponent(const glm::vec3 &halfExtents, float mass = 1.f, bool isStatic = false);

        void setHandle(uint32_t handle);
        uint32_t getHandle() const;

        const glm::vec3 &getHalfExtents() const;
        float getMass() const;
        bool getIsStatic() const;
    };
}

#endif