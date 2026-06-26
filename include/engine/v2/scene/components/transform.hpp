#ifndef __ENGINE_V2_SCENE_COMPONENTS_TRANSFORM_HPP
#define __ENGINE_V2_SCENE_COMPONENTS_TRANSFORM_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/resources/transform.hpp>
#include <engine/v2/scene/component.hpp>
#include <nlohmann/json.hpp>

namespace Engine::Scene::Components
{
    class Transform : public Component
    {
        Resources::Transform transform;

    public:
        inline Transform(const Resources::Transform &transform = {}) : transform(transform) {}

        inline Resources::Transform &getTransform() { return transform; }
        inline const Resources::Transform &getTransform() const { return transform; }
        inline void setTransform(const Resources::Transform &transform)
        {
            this->transform = transform;
        };
        std::string type() const override { return "Transform"; }
    };

    void to_json(nlohmann::json &j, const Transform &transform);
    void from_json(const nlohmann::json &j, Transform &transform);
}

#endif