#ifndef __ENGINE_SYSTEMS_RENDER_HPP
#define __ENGINE_SYSTEMS_RENDER_HPP

#include <engine/system.hpp>
#include <glm/glm.hpp>

namespace Engine::Systems
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