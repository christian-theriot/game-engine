#ifndef __EDITOR_SYSTEM_HPP
#define __EDITOR_SYSTEM_HPP

namespace Editor
{
    class World;

    class System
    {
    public:
        virtual ~System() = default;
        virtual void onAttach(World *world) {};
        virtual void onDetach(World *world) {};
        virtual void update(World *world, float deltaTime) {};
    };
}

#endif