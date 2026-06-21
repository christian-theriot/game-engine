#ifndef __ENGINE_BINDINGS_HPP
#define __ENGINE_BINDINGS_HPP

#include <engine/scripting.hpp>
#include <sol/sol.hpp>
#include <wasmtime.h>
#include <vector>

namespace Engine
{
    class World;

    template <typename T, typename State = World>
    struct IScriptBindings
    {
        virtual void bindAll(T &language, State *state) = 0;
    };

    struct LuaBindings : public IScriptBindings<sol::state>
    {
        void bindAll(sol::state &lua, World *world) override;
    };

    struct WasmBindings : public IScriptBindings<std::vector<wasmtime_extern_t>, Engine::WasmScriptingEngine>
    {
        void bindAll(std::vector<wasmtime_extern_t> &wasmExports, Engine::WasmScriptingEngine *self) override;
    };
}

#endif