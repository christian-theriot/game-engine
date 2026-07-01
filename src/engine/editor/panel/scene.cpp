#include <engine/editor/panel/scene.hpp>
#include <engine/scene/components/transform.hpp>
#include <engine/scene/components/rigidbody.hpp>
#include <engine/scene/systems/physics.hpp>
#include <imgui.h>

Engine::Editor::ScenePanel::ScenePanel(std::shared_ptr<Scene::World> world)
    : world(world), newEntityNameBuffer(256, '\0')
{
    strncpy(newEntityNameBuffer.data(), "New Entity", 10);
}
void Engine::Editor::ScenePanel::render()
{
    if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
    {
        renderAddEntityButton();
        ImGui::Separator();
        renderEntityList();
        ImGui::Separator();
        renderEntityProperties();
    }
    ImGui::End();
}
void Engine::Editor::ScenePanel::renderAddEntityButton()
{
    ImGui::InputText("Entity Name", newEntityNameBuffer.data(), newEntityNameBuffer.size());
    if (ImGui::Button("Add Entity"))
    {
        auto entity = world->createEntity();
        selectedEntityId = entity->getId();
        entity->setName(newEntityNameBuffer.data());
    }
}
void Engine::Editor::ScenePanel::renderEntityList()
{
    if (ImGui::BeginChild("Entity List", ImVec2(250, 300)))
    {
        for (const auto &entity : world->getEntities())
        {
            if (ImGui::Selectable(entity->getName().c_str(), selectedEntityId == entity->getId()))
            {
                selectedEntityId = entity->getId();
            }
        }
    }
    ImGui::EndChild();
}
void Engine::Editor::ScenePanel::renderEntityProperties()
{
    if (selectedEntityId != -1)
    {
        auto *entity = world->getEntityById(selectedEntityId);
        if (entity)
        {
            if (ImGui::BeginChild("Properties", ImVec2(250, 300)))
            {
                ImGui::Text("Entity: %s", entity->getName().c_str());
                ImGui::Separator();

                if (auto *transform = entity->getComponent<Scene::Components::Transform>())
                {
                    auto *rigidbody = entity->getComponent<Scene::Components::Rigidbody>();
                    auto *physics = world->getSystem<Scene::Systems::Physics>();

                    assert(physics && rigidbody && "Rigidbody component should exist if Transform component exists");

                    if (ImGui::TreeNode("Transform"))
                    {
                        glm::vec3 pos = transform->getTransform().position;
                        if (ImGui::DragFloat3("Position", &pos[0], 0.1f))
                        {
                            transform->getTransform().position = pos;
                            physics->setPosition(rigidbody->getHandle(), pos);
                        }

                        glm::vec3 euler = glm::degrees(glm::eulerAngles(transform->getTransform().rotation));
                        if (ImGui::DragFloat3("Rotation", &euler[0], 0.1f))
                        {
                            transform->getTransform().rotation = glm::quat(glm::radians(euler));
                            physics->setRotation(rigidbody->getHandle(), transform->getTransform().rotation);
                        }

                        glm::vec3 scale = transform->getTransform().scale;
                        if (ImGui::DragFloat3("Scale", &scale[0], 0.1f))
                        {
                            transform->getTransform().scale = scale;
                        }

                        ImGui::TreePop();
                    }
                }
            }
            ImGui::EndChild();
        }
    }
}