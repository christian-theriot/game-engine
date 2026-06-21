#ifndef __ENGINE_ENTITY_HPP
#define __ENGINE_ENTITY_HPP

#include <engine/component.hpp>
#include <unordered_map>
#include <string>

namespace Engine
{
    class Entity
    {
        uint64_t id;
        std::string uuid;
        std::unordered_map<size_t, std::shared_ptr<Component>> components;
        static uint64_t nextId;

    public:
        Entity();

        uint64_t getId() const;

        template <typename T, typename... Args>
        T *addComponent(Args &&...args)
        {
            static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

            size_t typeHash = typeid(T).hash_code();

            if (components.find(typeHash) != components.end())
            {
                throw std::runtime_error("Component already exists on this entity");
            }

            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            component->onAttach();
            components[typeHash] = component;

            return component.get();
        }

        template <typename T>
        T *getComponent() const
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = components.find(typeHash);

            if (item == components.end())
            {
                return nullptr;
            }

            return static_cast<T *>(item->second.get());
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

        const std::unordered_map<size_t, std::shared_ptr<Component>> &getComponents() const;
    };
}

#endif