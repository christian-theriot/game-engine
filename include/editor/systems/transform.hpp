#ifndef __EDITOR_SYSTEMS_TRANSFORM_HPP
#define __EDITOR_SYSTEMS_TRANSFORM_HPP

#include <editor/system.hpp>

namespace Editor::Systems
{
    class TransformSystem : public System
    {
    public:
        void update(World *world, float deltaTime) override;
    };
}

#endif