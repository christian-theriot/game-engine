#ifndef __ENGINE_SCENE_COMPONENT_HPP
#define __ENGINE_SCENE_COMPONENT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/serialization/serializable.hpp>
#include <string>

namespace Engine::Scene
{
    class Component : public Serialization::ISerializable
    {
    public:
        virtual ~Component() = default;
        virtual std::string type() const = 0;
        virtual void onAttach() {}
        virtual void onDetach() {}
    };
}

#endif