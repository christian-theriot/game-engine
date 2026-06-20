#ifndef __ENGINE_COMPONENT_HPP
#define __ENGINE_COMPONENT_HPP

namespace Engine
{
    class Component
    {
    public:
        virtual ~Component() = default;
        virtual void onAttach() {};
        virtual void onDetach() {};
        virtual void update(float deltaTime) {};
    };
}

#endif