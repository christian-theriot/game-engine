#include <engine/bindings.hpp>
#include <engine/components/transform.hpp>
#include <engine/transform.hpp>
#include <engine/entity.hpp>
#include <engine/world.hpp>

void Engine::ScriptBindings::bindAll(sol::state &lua, World *registry)
{
    // Logging
    lua.set_function("log", [](const std::string &message)
                     { std::cout << "[Lua] " << message << std::endl; });

    // glm::vec3
    lua.new_usertype<glm::vec3>("vec3",
                                sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
                                "x", &glm::vec3::x,
                                "y", &glm::vec3::y,
                                "z", &glm::vec3::z);
}