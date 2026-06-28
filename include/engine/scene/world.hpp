#ifndef __ENGINE_SCENE_WORLD_HPP
#define __ENGINE_SCENE_WORLD_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/entity.hpp>
#include <engine/scene/system.hpp>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <memory>

namespace Engine::Scene
{
    class World
    {
        std::vector<std::unique_ptr<Entity>> entities;
        std::unordered_map<size_t, std::unique_ptr<System>> systems;

        static uint64_t nextEntityId;

    public:
        Entity *createEntity();
        void destroyEntity(Entity *entity);

        inline const std::vector<std::unique_ptr<Entity>> &getEntities() const { return entities; }
        Entity *getEntityById(uint64_t id) const;

        void update();

        template <typename T, typename... Args>
        inline T *addSystem(Args &&...args)
        {
            static_assert(std::is_base_of_v<System, T>, "T must inherit from System");

            size_t typeHash = typeid(T).hash_code();

            if (systems.find(typeHash) != systems.end())
            {
                throw std::runtime_error("System already registered");
            }

            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            system->onAttach();
            systems[typeHash] = std::move(system);

            return static_cast<T *>(systems[typeHash].get());
        }

        template <typename T>
        inline T *getSystem() const
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
        inline bool hasSystem() const
        {
            size_t typeHash = typeid(T).hash_code();
            return systems.find(typeHash) != systems.end();
        }

        template <typename T>
        inline void removeSystem()
        {
            size_t typeHash = typeid(T).hash_code();
            auto item = systems.find(typeHash);

            if (item != systems.end())
            {
                item->second->onDetach();
                systems.erase(item);
            }
        }

        template <typename T>
        inline std::vector<Entity *> getEntitiesWithComponent() const
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

    void to_json(nlohmann::json &j, const World &world);
    void from_json(const nlohmann::json &j, World &world);
}

#endif