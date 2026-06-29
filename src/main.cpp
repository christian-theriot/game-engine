/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/version.hpp>
#include <engine/window.hpp>
#include <engine/events.hpp>
#include <engine/clock.hpp>
#include <engine/audio.hpp>
#include <engine/resources/shader.hpp>
#include <engine/resources/texture.hpp>
#include <engine/render/render-mesh.hpp>
#include <engine/scene/components/mesh.hpp>
#include <engine/scene/components/transform.hpp>
#include <engine/scene/components/rigidbody.hpp>
#include <engine/scene/components/script.hpp>
#include <engine/scene/entity.hpp>
#include <engine/scene/systems/physics.hpp>
#include <engine/scene/systems/script.hpp>
#include <engine/scene/world.hpp>
#include <engine/input.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    Engine::Version version(0, 2, 5);
    std::cout << "Game Engine v" << version.get() << std::endl;

    Engine::Window window;
    Engine::Scene::World world;
    std::ifstream file("world.json");
    nlohmann::json inJson;
    file >> inJson;
    inJson.get_to(world);

    // auto *lua = world.getSystem<Engine::Scene::Systems::LuaScript>();
    auto *wasm = world.getSystem<Engine::Scene::Systems::WasmScript>();
    Engine::Audio audio;
    Engine::Clock clock;
    Engine::EventBus events;
    Engine::Input input(&window, &events);

    auto audioEntities = world.getEntitiesWithComponent<Engine::Scene::Components::AudioSource>();

    for (auto *entity : audioEntities)
    {
        audio.add(entity);
    }

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
    audio.setListenerTransform(listenerTransform);

    auto shader = Engine::Resources::Shader::load("texture");
    auto texture = Engine::Resources::Texture::load("checkerboard-even.png");

    auto cubeEntity = world.getEntityById(1);
    auto planeEntity = world.getEntityById(2);

    cubeEntity->getRenderMesh().value().setProjection(projection);
    planeEntity->getRenderMesh().value().setProjection(projection);
    cubeEntity->getRenderMesh().value().setView(view);
    planeEntity->getRenderMesh().value().setView(view);

    events.subscribe<Engine::KeyEvent>([&](const Engine::KeyEvent &event)
                                       {
        if (event.getKey() == GLFW_KEY_LEFT && event.getAction() == GLFW_PRESS)
        {
            std::cout << "left" << std::endl;
            audio.play(cubeEntity);
        }
        else if (event.getKey() == GLFW_KEY_RIGHT && event.getAction() == GLFW_PRESS)
        {
            std::cout << "right" << std::endl;
            audio.play(cubeEntity);
        }
        else if (event.getKey() == GLFW_KEY_UP && event.getAction() == GLFW_PRESS)
        {
            std::cout << "up" << std::endl;
            audio.play(cubeEntity);
        }
        else if (event.getKey() == GLFW_KEY_DOWN && event.getAction() == GLFW_PRESS)
        {
            std::cout << "down" << std::endl;
            audio.play(cubeEntity);
        } });

    while (window.is_open())
    {
        clock.tick();
        events.tick();
        window.glDeclarations();

        if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window.close();
        }

        cubeEntity->getRenderMesh().value().render(cubeEntity->getComponent<Engine::Scene::Components::Transform>()->getTransform(), shader, texture);
        // cubeEntity.getRenderMesh().value().render(cubeEntity.getComponent<Engine::Scene::Components::Transform>()->getTransform(), shader, texture);
        planeEntity->getRenderMesh().value().render(planeEntity->getComponent<Engine::Scene::Components::Transform>()->getTransform(), shader, texture);

        audio.update(&world, clock.getDeltaTime());
        world.update(clock.getDeltaTime());
        window.tick();
    }

    return 0;
}