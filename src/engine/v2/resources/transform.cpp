#include <engine/v2/resources/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Engine::Resources::Transform::update()
{
    model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
}
glm::mat4 Engine::Resources::Transform::updated() const
{
    glm::mat4 out = glm::mat4(1.f);
    out = glm::translate(out, position);
    out *= glm::mat4_cast(rotation);
    out = glm::scale(out, scale);
    return out;
}
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