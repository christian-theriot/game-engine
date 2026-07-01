#include <engine/scene/entity.hpp>
#include <engine/scene/components/mesh.hpp>
#include <engine/scene/components/transform.hpp>
#include <engine/scene/components/rigidbody.hpp>
#include <engine/scene/components/script.hpp>
#include <engine/scene/components/audio-source.hpp>

void Engine::Scene::to_json(nlohmann::json &j, const Entity &entity)
{
    j["name"] = entity.getName();
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
            else if (component.second->type() == "Rigidbody")
            {
                j["components"].push_back(*dynamic_cast<Components::Rigidbody *>(component.second.get()));
            }
            else if (component.second->type() == "LuaScript")
            {
                j["components"].push_back(*dynamic_cast<Components::LuaScript *>(component.second.get()));
            }
            else if (component.second->type() == "WasmScript")
            {
                j["components"].push_back(*dynamic_cast<Components::WasmScript *>(component.second.get()));
            }
            else if (component.second->type() == "AudioSource")
            {
                j["components"].push_back(*dynamic_cast<Components::AudioSource *>(component.second.get()));
            }
            else
            {
                std::cerr << "Warning: Unknown component type '" << component.second->type() << "' during serialization." << std::endl;
            }
        }
    }
}
void Engine::Scene::from_json(const nlohmann::json &j, Entity &entity)
{
    entity.setName(j.at("name").get<std::string>());
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
            else if (type == "Rigidbody")
            {
                entity.addComponent<Components::Rigidbody>(component.get<Components::Rigidbody>());
            }
            else if (type == "LuaScript")
            {
                entity.addComponent<Components::LuaScript>(component.get<Components::LuaScript>());
            }
            else if (type == "WasmScript")
            {
                entity.addComponent<Components::WasmScript>(component.get<Components::WasmScript>());
            }
            else if (type == "AudioSource")
            {
                entity.addComponent<Components::AudioSource>(component.get<Components::AudioSource>());
            }
            else
            {
                std::cerr << "Warning: Unknown component type '" << type << "' during deserialization." << std::endl;
            }
        }
    }
}