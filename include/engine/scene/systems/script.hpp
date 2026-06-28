#ifndef __ENGINE_SCENE_SYSTEMS_SCRIPT_HPP
#define __ENGINE_SCENE_SYSTEMS_SCRIPT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/system.hpp>
#include <engine/scene/entity.hpp>
#include <sol/sol.hpp>
#include <wasmtime.h>

namespace Engine::Scene::Systems
{
    class LuaScript : public System
    {
        sol::state lua;
        World *world;

    public:
        LuaScript();
        ~LuaScript() override;

        inline void setWorld(World *world) { this->world = world; }

        bool load(Entity *entity);
        void update(World *world, float deltaTime) override;
    };

    class WasmScript : public System
    {
        World *world;

        void unload(Entity *entity);

    public:
        ~WasmScript() override;

        inline void setWorld(World *world) { this->world = world; }

        bool load(Entity *entity);
        void update(World *world, float deltaTime) override;
    };
}

#endif