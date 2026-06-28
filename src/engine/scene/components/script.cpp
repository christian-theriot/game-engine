#include <engine/scene/components/script.hpp>

void Engine::Scene::Components::to_json(nlohmann::json &j, const Engine::Scene::Components::LuaScript &s)
{
    j["path"] = s.scriptPath;
    j["type"] = s.type();
}
void Engine::Scene::Components::from_json(const nlohmann::json &j, Engine::Scene::Components::LuaScript &s)
{
    s.scriptPath = j.at("path").get<std::string>();
}

void Engine::Scene::Components::to_json(nlohmann::json &j, const Engine::Scene::Components::WasmScript &s)
{
    j["path"] = s.scriptPath;
    j["type"] = s.type();
}
void Engine::Scene::Components::from_json(const nlohmann::json &j, Engine::Scene::Components::WasmScript &s)
{
    s.scriptPath = j.at("path").get<std::string>();
}