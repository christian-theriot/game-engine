#include <editor/entity.hpp>

uint64_t Editor::Entity::nextId = 1;

Editor::Entity::Entity()
    : id(nextId++)
{
}
uint64_t Editor::Entity::getId() const
{
    return id;
}
const std::unordered_map<size_t, std::shared_ptr<Editor::Component>> &Editor::Entity::Entity::getComponents() const
{
    return components;
}