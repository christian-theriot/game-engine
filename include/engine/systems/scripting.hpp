#ifndef __ENGINE_SYSTEMS_SCRIPTING_HPP
#define __ENGINE_SYSTEMS_SCRIPTING_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/scripting.hpp>
#include <engine/system.hpp>
#include <engine/components/scripting.hpp>
#include <memory>

namespace Engine::Systems
{
    class LuaScriptingSystem : public System
    {
        std::unique_ptr<Engine::LuaScriptingEngine> scriptingEngine;
        World *world;

    public:
        explicit LuaScriptingSystem(World *world);
        ~LuaScriptingSystem();

        void init();
        void update(World *world, float deltaTime);

        bool load(Engine::Components::LuaScriptComponent *component, const std::string &path);

        LuaScriptingEngine &getScriptingEngine();
    };

    class WasmScriptingSystem : public System
    {
        World *world;

    public:
        explicit WasmScriptingSystem(World *world);

        void update(World *world, float deltaTime);

        // Compile + instantiate a .wasm module onto a component.
        bool load(Engine::Components::WasmScriptComponent *component, const std::string &path);
    };
}

#endif