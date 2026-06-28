#include <engine/scene/components/mesh.hpp>

void Engine::Scene::Components::to_json(nlohmann::json &j, const Mesh &mesh)
{
    j["mesh"] = mesh.getMesh();
    j["type"] = mesh.type();
}
void Engine::Scene::Components::from_json(const nlohmann::json &j, Mesh &mesh)
{
    if (j.contains("mesh"))
    {
        mesh.setMesh(j.at("mesh").get<Resources::Mesh>());
    }
}