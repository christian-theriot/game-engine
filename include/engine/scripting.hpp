#ifndef __ENGINE_SCRIPTING_HPP
#define __ENGINE_SCRIPTING_HPP

#include <sol/sol.hpp>
#include <string>

namespace Engine
{
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
}
#endif