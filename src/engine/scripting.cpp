#include <engine/scripting.hpp>
#include <iostream>

Engine::LuaScriptingEngine::LuaScriptingEngine()
{
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
}
bool Engine::LuaScriptingEngine::load(const std::string &scriptPath)
{
    sol::protected_function_result result = lua.safe_script_file(scriptPath, sol::script_pass_on_error);

    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[Lua] Failed to load '" << scriptPath << "': " << err.what() << std::endl;
        return false;
    }
    return true;
}
bool Engine::LuaScriptingEngine::run(const std::string &code)
{
    sol::protected_function_result result = lua.safe_script(code, sol::script_pass_on_error);

    if (!result.valid())
    {
        sol::error err = result;
        std::cerr << "[Lua] Runtime error: " << err.what() << std::endl;
        return false;
    }
    return true;
}
sol::state &Engine::LuaScriptingEngine::state()
{
    return lua;
}
