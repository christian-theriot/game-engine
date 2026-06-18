#ifndef __EDITOR_SYSTEMS_RENDER_HPP
#define __EDITOR_SYSTEMS_RENDER_HPP

#include <editor/system.hpp>
#include <glm/glm.hpp>

namespace Editor::Systems
{
    class RenderSystem : public System
    {
        glm::mat4 viewProjection;

    public:
        void setViewProjection(const glm::mat4 &view);
        void update(World *world, float deltaTime) override;
    };
}

#endif