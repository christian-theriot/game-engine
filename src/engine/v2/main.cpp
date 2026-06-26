/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/version.hpp>
#include <engine/v2/scene/systems/window.hpp>
#include <engine/v2/scene/systems/events.hpp>
#include <engine/v2/scene/systems/clock.hpp>
#include <engine/v2/resources/shader.hpp>
#include <engine/v2/resources/texture.hpp>
#include <engine/v2/render/render-mesh.hpp>
#include <engine/v2/scene/components/mesh.hpp>
#include <engine/v2/scene/components/transform.hpp>
#include <engine/v2/scene/components/rigidbody.hpp>
#include <engine/v2/scene/entity.hpp>
#include <engine/v2/scene/systems/physics.hpp>
#include <engine/v2/scene/world.hpp>
#include <engine/v2/scene/systems/input.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    Engine::Version version(0, 2, 3);
    std::cout << "Game Engine v" << version.get() << std::endl;

    // std::ifstream inFile("entity.json");
    // nlohmann::json inJson;
    // inFile >> inJson;
    // auto cube = inJson.at("cube").get<Engine::Scene::Entity>();
    // auto plane = inJson.at("plane").get<Engine::Scene::Entity>();

    // Engine::Scene::World world;

    // auto cubeEntity = world.createEntity();
    // cubeEntity->addComponent<Engine::Scene::Components::Mesh>(cube.getComponent<Engine::Scene::Components::Mesh>()->getMesh());
    // cubeEntity->addComponent<Engine::Scene::Components::Transform>();
    // cubeEntity->addComponent<Engine::Scene::Components::Rigidbody>(glm::vec3(0.5f, 0.5f, 0.5f), 1.f, false, 0.5f, 0.5f);

    // cubeEntity->getComponent<Engine::Scene::Components::Transform>()->setTransform(cube.getComponent<Engine::Scene::Components::Transform>()->getTransform());

    // auto planeEntity = world.createEntity();
    // planeEntity->addComponent<Engine::Scene::Components::Mesh>(plane.getComponent<Engine::Scene::Components::Mesh>()->getMesh());
    // planeEntity->addComponent<Engine::Scene::Components::Transform>();
    // planeEntity->addComponent<Engine::Scene::Components::Rigidbody>(glm::vec3(0.5f, 0.5f, 0.5f), 1.f, true, 0.05f, 0.5f);

    // planeEntity->getComponent<Engine::Scene::Components::Transform>()->setTransform(plane.getComponent<Engine::Scene::Components::Transform>()->getTransform());

    // auto *physics = world.addSystem<Engine::Scene::Systems::Physics>();

    // auto cubeHandle = physics->add(cubeEntity);
    // auto planeHandle = physics->add(planeEntity);

    // std::ofstream file("world.json");
    // nlohmann::json outJson = world;
    // file << outJson.dump(4);

    Engine::Scene::World world;
    std::ifstream file("world.json");
    nlohmann::json inJson;
    file >> inJson;
    world = inJson.get<Engine::Scene::World>();

    auto projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
    auto view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

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
        }
        else if (event.getKey() == GLFW_KEY_RIGHT && event.getAction() == GLFW_PRESS)
        {
            std::cout << "right" << std::endl;
        }
        else if (event.getKey() == GLFW_KEY_UP && event.getAction() == GLFW_PRESS)
        {
            std::cout << "up" << std::endl;
        }
        else if (event.getKey() == GLFW_KEY_DOWN && event.getAction() == GLFW_PRESS)
        {
            std::cout << "down" << std::endl;
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