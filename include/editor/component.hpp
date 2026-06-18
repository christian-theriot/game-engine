#ifndef __EDITOR_COMPONENT_HPP
#define __EDITOR_COMPONENT_HPP

namespace Editor
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