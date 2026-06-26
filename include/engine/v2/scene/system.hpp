#ifndef __ENGINE_V2_SCENE_SYSTEM_HPP
#define __ENGINE_V2_SCENE_SYSTEM_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

namespace Engine::Scene
{
    class World;

    class System
    {
    public:
        virtual ~System() = default;
        virtual void update(World *world, float deltaTime) = 0;
        virtual void onAttach() {}
        virtual void onDetach() {}
    };
}

#endif