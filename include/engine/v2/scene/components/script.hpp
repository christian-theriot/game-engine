#ifndef __ENGINE_V2_SCENE_COMPONENTS_SCRIPT_HPP
#define __ENGINE_V2_SCENE_COMPONENTS_SCRIPT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/scene/component.hpp>

namespace Engine::Scene::Components
{
    struct ScriptComponent : public Component
    {
        std::string scriptPath;
    };
}

#endif