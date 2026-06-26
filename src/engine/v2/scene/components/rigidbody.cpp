#include <engine/v2/scene/components/rigidbody.hpp>
#include <engine/v2/resources/transform.hpp>

void Engine::Scene::Components::to_json(nlohmann::json &j, const Rigidbody &rigidbody)
{
    j["halfExtents"] = rigidbody.getHalfExtents();
    j["mass"] = rigidbody.getMass();
    j["static"] = rigidbody.getIsStatic();
    j["restitution"] = rigidbody.getRestitution();
    j["friction"] = rigidbody.getFriction();
    j["type"] = rigidbody.type();
}
void Engine::Scene::Components::from_json(const nlohmann::json &j, Rigidbody &rigidbody)
{
    rigidbody.setHalfExtents(j.at("halfExtents").get<glm::vec3>());
    rigidbody.setMass(j.at("mass").get<float>());
    rigidbody.setIsStatic(j.at("static").get<bool>());
    rigidbody.setRestitution(j.at("restitution").get<float>());
    rigidbody.setFriction(j.at("friction").get<float>());
}