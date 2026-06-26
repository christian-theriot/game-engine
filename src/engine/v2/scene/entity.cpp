#include <engine/v2/scene/entity.hpp>
#include <engine/v2/scene/components/mesh.hpp>
#include <engine/v2/scene/components/transform.hpp>

void Engine::Scene::to_json(nlohmann::json &j, const Entity &entity)
{
    j["components"] = nlohmann::json::array();

    for (const auto &component : entity.getComponents())
    {
        if (component.second)
        {
            if (component.second->type() == "Transform")
            {
                j["components"].push_back(*dynamic_cast<Components::Transform *>(component.second.get()));
            }
            else if (component.second->type() == "Mesh")
            {
                j["components"].push_back(*dynamic_cast<Components::Mesh *>(component.second.get()));
            }
        }
    }
}
void Engine::Scene::from_json(const nlohmann::json &j, Entity &entity)
{
    for (const auto &component : j.at("components"))
    {
        if (component.contains("type"))
        {
            const std::string type = component.at("type").get<std::string>();
            if (type == "Transform")
            {
                entity.addComponent<Components::Transform>(component.get<Components::Transform>());
            }
            else if (type == "Mesh")
            {
                entity.addComponent<Components::Mesh>(component.get<Components::Mesh>());
            }
        }
    }
}