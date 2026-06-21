#ifndef __ENGINE_BINDINGS_HPP
#define __ENGINE_BINDINGS_HPP

#include <sol/sol.hpp>

namespace Engine
{
    class World;

    namespace ScriptBindings
    {
        void bindAll(sol::state &lua, World *world);
    }
}

#endif