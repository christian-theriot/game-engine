#ifndef __ENGINE_SCENE_COMPONENTS_SCRIPT_HPP
#define __ENGINE_SCENE_COMPONENTS_SCRIPT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/component.hpp>
#include <sol/sol.hpp>
#include <wasmtime.h>
#include <nlohmann/json.hpp>

namespace Engine::Scene::Components
{
    struct LuaScript : public Component
    {
        std::string scriptPath;
        bool loaded = false;

        sol::table instance;

        LuaScript(const std::string &path = "") : scriptPath(path) {}
        std::string type() const override { return "LuaScript"; }
    };

    struct WasmScript : public Component
    {
        std::string scriptPath;
        bool loaded = false;

        wasm_engine_t *engine = nullptr;
        wasmtime_store_t *store = nullptr;
        wasmtime_module_t *module = nullptr;
        wasmtime_instance_t instance = {};
        bool hasInstance = false;

        wasmtime_func_t onStart = {};
        bool hasOnStart = false;

        wasmtime_func_t onUpdate = {};
        bool hasOnUpdate = false;

        WasmScript(const std::string &path = "") : scriptPath(path) {}
        std::string type() const override { return "WasmScript"; }
    };

    void to_json(nlohmann::json &j, const LuaScript &s);
    void from_json(const nlohmann::json &j, LuaScript &s);

    void to_json(nlohmann::json &j, const WasmScript &s);
    void from_json(const nlohmann::json &j, WasmScript &s);
}

#endif