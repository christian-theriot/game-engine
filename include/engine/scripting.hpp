#ifndef __ENGINE_SCRIPTING_HPP
#define __ENGINE_SCRIPTING_HPP

#include <sol/sol.hpp>
#include <wasmtime.h>
#include <string>

namespace Engine
{
    class World;

    class IScriptingEngine
    {
    public:
        virtual ~IScriptingEngine() = default;

        // Load/compile a script from a file. Returns false on failure
        virtual bool load(const std::string &filePath) = 0;

        // Run an arbitrary string of script code
        virtual bool run(const std::string &code) = 0;
    };

    class LuaScriptingEngine : public IScriptingEngine
    {
        sol::state lua;

    public:
        LuaScriptingEngine();

        bool load(const std::string &filePath) override;
        bool run(const std::string &code) override;

        // Direct access for binding engine types (see ScriptBindings)
        sol::state &state();
    };

    class WasmScriptingEngine : public IScriptingEngine
    {
        wasm_engine_t *engine;
        wasmtime_store_t *store;
        wasmtime_context_t *context;
        wasmtime_module_t *module;
        wasmtime_instance_t instance;
        bool instantiated;
        World *world;

    public:
        WasmScriptingEngine();
        ~WasmScriptingEngine();

        bool load(const std::string &filePath) override;
        bool run(const std::string &code) override { return false; };

        bool call(const std::string &name);
        bool call(const std::string &name, float arg);

        bool hasExport(const std::string &name) const;

        wasmtime_context_t *getContext() const { return context; }
        World *getWorld() const { return world; }
        void setWorld(World *world) { this->world = world; }
    };
}
#endif