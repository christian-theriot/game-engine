#ifndef __ENGINE_TRANSFORM_HPP
#define __ENGINE_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
    /**
     * TODO:
     * 1. Animation/tweening support (setTargetPosition, etc)
     * 2. Quaternion rotation option (currently Euler angles)
     * 3. Local-to-world/world-to-local conversion utilities
     */
    class Transform
    {
        glm::vec3 position, rotation, scale;
        glm::mat4 localMatrix, worldMatrix;
        bool isDirty;

        void updateMatrix();

    public:
        Transform();

        void setPosition(const glm::vec3 &pos);
        void setRotation(const glm::vec3 &rot);
        void setRotation(const glm::quat &rot);
        void setScale(const glm::vec3 &newScale);

        void update(const glm::mat4 &parentMatrix = 1.f);

        glm::vec3 getPosition() const;
        glm::vec3 getRotation() const;
        glm::vec3 getScale() const;
        glm::vec3 getWorldPosition() const;

        glm::mat4 getLocalMatrix() const;
        glm::mat4 getWorldMatrix() const;
    };
}

#endif