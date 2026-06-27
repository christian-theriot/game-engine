#ifndef __ENGINE_V2_RESOURCES_TRANSFORM_HPP
#define __ENGINE_V2_RESOURCES_TRANSFORM_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/serialization/serializable.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

namespace glm
{
    inline void to_json(nlohmann::json &j, const glm::vec3 &vec)
    {
        j = {vec.x, vec.y, vec.z};
    }
    inline void from_json(const nlohmann::json &j, glm::vec3 &vec)
    {
        vec.x = j.at(0).get<GLfloat>();
        vec.y = j.at(1).get<GLfloat>();
        vec.z = j.at(2).get<GLfloat>();
    }

    inline void to_json(nlohmann::json &j, const glm::quat &quat)
    {
        j = {quat.w, quat.x, quat.y, quat.z};
    }
    inline void from_json(const nlohmann::json &j, glm::quat &quat)
    {
        quat.w = j.at(0).get<GLfloat>();
        quat.x = j.at(1).get<GLfloat>();
        quat.y = j.at(2).get<GLfloat>();
        quat.z = j.at(3).get<GLfloat>();
    }
}

namespace Engine::Resources
{
    class Transform : public Serialization::ISerializable
    {
        glm::vec3 lastPosition;
        glm::vec3 lastVelocity;

    public:
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 velocity = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 acceleration = glm::vec3(0.f, 0.f, 0.f);
        glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
        glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
        glm::mat4 model = glm::mat4(1.f);

        void update();
        glm::mat4 updated() const;
    };

    void to_json(nlohmann::json &j, const Transform &transform);
    void from_json(const nlohmann::json &j, Transform &transform);
}

#endif