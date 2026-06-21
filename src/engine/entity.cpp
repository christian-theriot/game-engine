/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/entity.hpp>
#include <engine/components/scripting.hpp>
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
const std::unordered_map<size_t, std::shared_ptr<Engine::Component>> &Engine::Entity::getComponents() const
{
    return components;
}