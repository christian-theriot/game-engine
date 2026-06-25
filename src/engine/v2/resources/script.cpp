#include <engine/v2/resources/script.hpp>

void Engine::Resources::to_json(nlohmann::json &j, const Script &script)
{
    j["source"] = script.source;
}
void Engine::Resources::from_json(const nlohmann::json &j, Script &script)
{
    script.source = j.at("source").get<std::string>();
}