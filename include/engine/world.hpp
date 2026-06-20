#ifndef __ENGINE_WORLD_HPP
#define __ENGINE_WORLD_HPP

#include <engine/entity.hpp>
#include <engine/system.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>

namespace Engine
{
    class World
    {
        std::vector<std::shared_ptr<Entity>> entities;
        std::unordered_map<size_t, std::shared_ptr<System>> systems;

    public:
        Entity *createEntity();
        void destroyEntity(Entity *entity);

        const std::vector<std::shared_ptr<Entity>> &getEntities() const;

        template <typename T, typename... Args>
        T *registerSystem(Args &&...args)
        {
            static_assert(std::is_base_of_v<System, T>, "T must inherit from System");

            size_t typeHash = typeid(T).hash_code();

            if (systems.find(typeHash) != systems.end())
            {
                throw std::runtime_error("System already registered");
            }

            auto system = std::make_shared<T>(std::forward<Args>(args)...);
            system->onAttach(this);
            systems[typeHash] = system;

            return system.get();
        }

        template <typename T>
        T *getSystem() const
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = systems.find(typeHash);

            if (item == systems.end())
            {
                return nullptr;
            }

            return static_cast<T *>(item->second.get());
        }

        template <typename T>
        void unregisterSystem()
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = systems.find(typeHash);

            if (item != systems.end())
            {
                item->second->onDetach(this);
                systems.erase(item);
            }
        }

        void update(float deltaTime);

        template <typename T>
        std::vector<Entity *> getEntitiesWithComponent()
        {
            std::vector<Entity *> result;

            for (auto &entity : entities)
            {
                if (entity->hasComponent<T>())
                {
                    result.push_back(entity.get());
                }
            }

            return result;
        }
    };
}

#endif