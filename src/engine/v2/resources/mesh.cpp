#include <engine/v2/resources/mesh.hpp>

void Engine::Resources::to_json(nlohmann::json &j, const Mesh &mesh)
{
    if (!mesh.vertices.empty())
    {
        j["vertices"] = mesh.vertices;
    }
    if (!mesh.uvs.empty())
    {
        j["uvs"] = mesh.uvs;
    }
    if (!mesh.colors.empty())
    {
        j["colors"] = mesh.colors;
    }
    if (!mesh.indices.empty())
    {
        j["indices"] = mesh.indices;
    }
}
void Engine::Resources::from_json(const nlohmann::json &j, Mesh &mesh)
{
    if (j.contains("vertices"))
    {
        mesh.vertices = j.at("vertices").get<std::vector<GLfloat>>();
    }

    if (j.contains("uvs"))
    {
        mesh.uvs = j.at("uvs").get<std::vector<GLfloat>>();
    }

    if (j.contains("colors"))
    {
        mesh.colors = j.at("colors").get<std::vector<GLfloat>>();
    }

    if (j.contains("indices"))
    {
        mesh.indices = j.at("indices").get<std::vector<GLuint>>();
    }
}