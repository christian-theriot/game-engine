#ifndef __ENGINE_V2_SCENE_ENTITY_HPP
#define __ENGINE_V2_SCENE_ENTITY_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/scene/component.hpp>
#include <engine/v2/scene/components/mesh.hpp>
#include <engine/v2/render/render-mesh.hpp>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace Engine::Scene
{
    class Entity
    {
        std::unordered_map<size_t, std::unique_ptr<Component>> components;
        Core::Result<Render::RenderMesh> renderMesh = Core::Fail<Render::RenderMesh>("RenderMesh not loaded");

    public:
        inline Entity() = default;

        template <typename T, typename... Args>
        inline T *addComponent(Args &&...args)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

            size_t typeHash = typeid(T).hash_code();

            if (components.find(typeHash) != components.end())
            {
                throw std::runtime_error("Component already exists on this entity");
            }

            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            component->onAttach();

            if constexpr (std::is_same_v<T, Components::Mesh>)
            {
                renderMesh = Render::RenderMesh::load(component->getMesh());
            }

            T *raw = component.get();
            components[typeHash] = std::move(component);

            return raw;
        }

        template <typename T, typename V = std::enable_if_t<std::is_base_of<Component, T>::value>>
        inline T *getComponent()
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = components.find(typeHash);

            if (item == components.end())
            {
                return nullptr;
            }

            return static_cast<T *>(item->second.get());
        }

        template <typename T, typename V = std::enable_if_t<std::is_base_of<Component, T>::value>>
        inline const T *getComponent() const
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = components.find(typeHash);

            if (item == components.end())
            {
                return nullptr;
            }

            return static_cast<const T *>(item->second.get());
        }

        template <typename T>
        bool hasComponent() const
        {
            return getComponent<T>() != nullptr;
        }

        template <typename T>
        void removeComponent()
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = components.find(typeHash);

            if (item != components.end())
            {
                item->second->onDetach();
                components.erase(item);
            }
        }

        inline const std::unordered_map<size_t, std::unique_ptr<Component>> &getComponents() const
        {
            return components;
        }

        inline Core::Result<Render::RenderMesh> &getRenderMesh()
        {
            return renderMesh;
        }

        inline const Core::Result<Render::RenderMesh> &getRenderMesh() const
        {
            return renderMesh;
        }
    };

    void to_json(nlohmann::json &j, const Entity &entity);
    void from_json(const nlohmann::json &j, Entity &entity);
}

#endif