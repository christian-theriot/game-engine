#ifndef __EDITOR_COMPONENTS_TRANSFORM_HPP
#define __EDITOR_COMPONENTS_TRANSFORM_HPP

#include <editor/component.hpp>
#include <editor/transform.hpp>

namespace Editor::Components
{
    class TransformComponent : public Component
    {
        std::unique_ptr<Transform> transform;

    public:
        TransformComponent(const glm::vec3 &pos = glm::vec3(0), const glm::vec3 &rot = glm::vec3(0), const glm::vec3 &scale = glm::vec3(1));

        Transform *getTransform() const;
        void setTransform(std::unique_ptr<Transform> transform);
    };
}

#endif