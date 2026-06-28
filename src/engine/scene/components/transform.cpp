#include <engine/scene/components/transform.hpp>

void Engine::Scene::Components::to_json(nlohmann::json &j, const Transform &transform)
{
    j["transform"] = transform.getTransform();
    j["type"] = transform.type();
}
void Engine::Scene::Components::from_json(const nlohmann::json &j, Transform &transform)
{
    if (j.contains("transform"))
    {
        transform.setTransform(j.at("transform").get<Resources::Transform>());
    }
}