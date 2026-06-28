#include <engine/scene/components/audio-source.hpp>

void Engine::Scene::Components::to_json(nlohmann::json &j, const AudioSource &component)
{
    j["path"] = component.getClipPath();
    j["vol"] = component.getVolume();
    j["pitch"] = component.getPitch();
    j["loop"] = component.isLooping();
    j["autoplay"] = component.shouldPlayOnStart();
    j["type"] = component.type();
}
void Engine::Scene::Components::from_json(const nlohmann::json &j, AudioSource &component)
{
    component.setClipPath(j.at("path").get<std::string>());
    component.setVolume(j.at("vol").get<float>());
    component.setPitch(j.at("pitch").get<float>());
    component.setLooping(j.at("loop").get<bool>());
    component.setPlayOnStart(j.at("autoplay").get<bool>());
}