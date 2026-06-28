/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/version.hpp>
#include <engine/v2/scene/systems/window.hpp>
#include <engine/v2/scene/systems/events.hpp>
#include <engine/v2/scene/systems/clock.hpp>
#include <engine/v2/scene/systems/audio.hpp>
#include <engine/v2/resources/shader.hpp>
#include <engine/v2/resources/texture.hpp>
#include <engine/v2/render/render-mesh.hpp>
#include <engine/v2/scene/components/mesh.hpp>
#include <engine/v2/scene/components/transform.hpp>
#include <engine/v2/scene/components/rigidbody.hpp>
#include <engine/v2/scene/components/script.hpp>
#include <engine/v2/scene/entity.hpp>
#include <engine/v2/scene/systems/physics.hpp>
#include <engine/v2/scene/systems/script.hpp>
#include <engine/v2/scene/world.hpp>
#include <engine/v2/scene/systems/input.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    Engine::Version version(0, 2, 3);
    std::cout << "Game Engine v" << version.get() << std::endl;

    Engine::Scene::World world;
    std::ifstream file("world.json");
    nlohmann::json inJson;
    file >> inJson;
    inJson.get_to(world);

    // auto *lua = world.getSystem<Engine::Scene::Systems::LuaScript>();
    auto *wasm = world.getSystem<Engine::Scene::Systems::WasmScript>();
    auto *audio = world.getSystem<Engine::Scene::Systems::Audio>();
    auto wasmEntities = world.getEntitiesWithComponent<Engine::Scene::Components::WasmScript>();

    if (wasm)
    {
        wasm->setWorld(&world);
        for (auto *entity : wasmEntities)
        {
            wasm->load(entity);
        }
    }

    auto projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
    auto view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    Engine::Resources::Transform listenerTransform;

    listenerTransform.position = glm::vec3(3, 3, 3);
    audio->setListenerTransform(listenerTransform);

    auto shader = Engine::Resources::Shader::load("texture");
    auto texture = Engine::Resources::Texture::load("checkerboard-even.png");

    auto cubeEntity = world.getEntityById(1);
    auto planeEntity = world.getEntityById(2);

    cubeEntity->getRenderMesh().value().setProjection(projection);
    planeEntity->getRenderMesh().value().setProjection(projection);
    cubeEntity->getRenderMesh().value().setView(view);
    planeEntity->getRenderMesh().value().setView(view);

    auto events = world.getSystem<Engine::Scene::Systems::EventBus>();
    auto window = world.getSystem<Engine::Scene::Systems::Window>();

    events->subscribe<Engine::KeyEvent>([&](const Engine::KeyEvent &event)
                                        {
        if (event.getKey() == GLFW_KEY_LEFT && event.getAction() == GLFW_PRESS)
        {
            std::cout << "left" << std::endl;
            audio->play(cubeEntity);
        }
        else if (event.getKey() == GLFW_KEY_RIGHT && event.getAction() == GLFW_PRESS)
        {
            std::cout << "right" << std::endl;
            audio->play(cubeEntity);
        }
        else if (event.getKey() == GLFW_KEY_UP && event.getAction() == GLFW_PRESS)
        {
            std::cout << "up" << std::endl;
            audio->play(cubeEntity);
        }
        else if (event.getKey() == GLFW_KEY_DOWN && event.getAction() == GLFW_PRESS)
        {
            std::cout << "down" << std::endl;
            audio->play(cubeEntity);
        } });

    while (window->is_open())
    {
        window->glDeclarations();

        if (glfwGetKey(window->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window->close();
        }

        cubeEntity->getRenderMesh().value().render(cubeEntity->getComponent<Engine::Scene::Components::Transform>()->getTransform(), shader, texture);
        // cubeEntity.getRenderMesh().value().render(cubeEntity.getComponent<Engine::Scene::Components::Transform>()->getTransform(), shader, texture);
        planeEntity->getRenderMesh().value().render(planeEntity->getComponent<Engine::Scene::Components::Transform>()->getTransform(), shader, texture);

        world.update();
    }

    return 0;
}