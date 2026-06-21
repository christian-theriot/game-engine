#ifndef __ENGINE_RENDERABLE_HPP
#define __ENGINE_RENDERABLE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glm/glm.hpp>

namespace Engine
{
    class Renderable
    {
    public:
        virtual void render(glm::mat4 mvp) const = 0;
    };
}

#endif