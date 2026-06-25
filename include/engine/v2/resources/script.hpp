#ifndef __ENGINE_V2_RESOURCES_SCRIPT_HPP
#define __ENGINE_V2_RESOURCES_SCRIPT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/serialization/serializable.hpp>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine::Resources
{
    struct Script : public Serialization::ISerializable
    {
        std::string source;
    };

    void to_json(nlohmann::json &j, const Script &script);
    void from_json(const nlohmann::json &j, Script &script);
}

#endif