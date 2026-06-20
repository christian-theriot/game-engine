#ifndef __ENGINE_SYSTEMS_TRANSFORM_HPP
#define __ENGINE_SYSTEMS_TRANSFORM_HPP

#include <engine/system.hpp>

namespace Engine::Systems
{
    class TransformSystem : public System
    {
    public:
        void update(World *world, float deltaTime) override;
    };
}

#endif