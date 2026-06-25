#include <engine/v2/resources/transform.hpp>

void Engine::Resources::to_json(nlohmann::json &j, const Transform &transform)
{
    j["position"] = transform.position;
    j["rotation"] = transform.rotation;
    j["scale"] = transform.scale;
}

void Engine::Resources::from_json(const nlohmann::json &j, Transform &transform)
{
    transform.position = j.at("position").get<glm::vec3>();
    transform.rotation = j.at("rotation").get<glm::quat>();
    transform.scale = j.at("scale").get<glm::vec3>();
}