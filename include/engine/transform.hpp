#ifndef __ENGINE_TRANSFORM_HPP
#define __ENGINE_TRANSFORM_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

namespace glm
{
    inline void to_json(nlohmann::json &j, const glm::vec3 &v)
    {
        j = nlohmann::json{v.x, v.y, v.z};
    }
    inline void from_json(const nlohmann::json &j, glm::vec3 &v)
    {
        if (j.is_array() && j.size() == 3)
        {
            j.at(0).get_to(v.x);
            j.at(1).get_to(v.y);
            j.at(2).get_to(v.z);
        }
    }
}

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

    void to_json(nlohmann::json &j, const Transform &t);
    void from_json(const nlohmann::json &j, Transform &t);
}

#endif