/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/entity.hpp>
#include <engine/components/scripting.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/mesh.hpp>
#include <engine/scripting.hpp>

uint64_t Engine::Entity::nextId = 1;

Engine::Entity::Entity()
    : id(nextId++)
{
}
uint64_t Engine::Entity::getId() const
{
    return id;
}
void Engine::Entity::setId(uint64_t newId)
{
    id = newId;
}
const std::unordered_map<size_t, std::shared_ptr<Engine::Component>> &Engine::Entity::getComponents() const
{
    return components;
}
void Engine::to_json(nlohmann::json &j, const Entity &entity)
{
    j["id"] = entity.getId();

    j["components"] = nlohmann::json::array();
    if (entity.hasComponent<Components::MeshComponent>())
    {
        j["components"].push_back(*entity.getComponent<Components::MeshComponent>());
    }
    if (entity.hasComponent<Components::TransformComponent>())
    {
        j["components"].push_back(*entity.getComponent<Components::TransformComponent>());
    }
}
void Engine::from_json(const nlohmann::json &j, Entity &entity)
{
    entity.setId(j.at("id").get<uint64_t>());

    for (const auto &componentJson : j.at("components"))
    {
        if (componentJson.contains("mesh"))
        {
            auto meshComponent = componentJson.get<Engine::Components::MeshComponent>();
            entity.addComponent<Engine::Components::MeshComponent>(std::move(meshComponent));
        }
        else if (componentJson.contains("transform"))
        {
            auto transformComponent = componentJson.get<Engine::Components::TransformComponent>();
            entity.addComponent<Engine::Components::TransformComponent>(std::move(transformComponent));
        }
    }
}