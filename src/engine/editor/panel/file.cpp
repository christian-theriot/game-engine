#include <engine/editor/panel/file.hpp>
#include <imgui.h>
#include <fstream>
#include <nlohmann/json.hpp>

Engine::Editor::FilePanel::FilePanel(std::shared_ptr<Scene::World> world, EventBus *events)
    : world(world),
      events(events),
      currentFilePath("world.json")
{
}
void Engine::Editor::FilePanel::render()
{
    std::vector<char> currentFilePathBuffer(currentFilePath.begin(), currentFilePath.end());

    ImGui::SetNextWindowPos(ImVec2(350.0f, 100.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 120.0f), ImGuiCond_Always);

    if (ImGui::Begin("File", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
    {
        ImGui::InputText("File Path", currentFilePathBuffer.data(), currentFilePathBuffer.size());

        if (ImGui::Button("Save"))
        {
            nlohmann::json j = *world;
            std::ofstream file(currentFilePath);
            if (file.is_open())
            {
                file << j.dump(4);
                file.close();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            std::ifstream file(currentFilePath);
            if (file.is_open())
            {
                nlohmann::json j;
                file >> j;
                file.close();
                Engine::Scene::World newWorld;
                j.get_to(newWorld);

                FileLoadEvent event;
                event.world = std::make_shared<Scene::World>(std::move(newWorld));
                events->publish(event);
            }
        }
    }
    ImGui::End();
}