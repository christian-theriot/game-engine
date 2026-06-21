#ifndef __ENGINE_COMPONENTS_SCRIPTING_HPP
#define __ENGINE_COMPONENTS_SCRIPTING_HPP

#include <engine/component.hpp>
#include <sol/sol.hpp>
#include <string>

namespace Engine::Components
{
    struct LuaScriptComponent : public Component
    {
        std::string scriptPath;

        sol::function onStart;
        sol::function onUpdate;

        sol::table self;
        bool started = false;
    };
}

#endif