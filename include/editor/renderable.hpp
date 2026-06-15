#ifndef __EDITOR_RENDERABLE_HPP
#define __EDITOR_RENDERABLE_HPP

#include <glm/glm.hpp>

namespace Editor
{
    class Renderable
    {
    public:
        virtual void render(glm::mat4 mvp) const = 0;
    };
}

#endif