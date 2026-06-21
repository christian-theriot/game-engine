#ifndef __ENGINE_COMPONENTS_SCRIPTING_HPP
#define __ENGINE_COMPONENTS_SCRIPTING_HPP

#include <engine/component.hpp>
#include <engine/scripting.hpp>
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

    struct WasmScriptComponent : public Component
    {
        std::string scriptPath;

        // Each component owns its own WASM instance so per-entity
        // state (module globals) doesn't leak between entities
        std::unique_ptr<Engine::WasmScriptingEngine> scriptingEngine;

        bool hasOnStart = false;
        bool hasOnUpdate = false;
        bool started = false;
    };
}

#endif