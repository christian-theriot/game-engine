/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/v1/version.hpp>
#include <engine/v1/window.hpp>
#include <engine/v1/camera.hpp>
#include <engine/v1/image.hpp>
#include <engine/v1/primitives/cube.hpp>
#include <engine/v1/primitives/plane.hpp>
#include <engine/v1/primitives/sphere.hpp>
#include <engine/v1/primitives/line.hpp>
#include <engine/v1/clock.hpp>
#include <engine/v1/resource-manager.hpp>
#include <engine/v1/systems/transform.hpp>
#include <engine/v1/systems/render.hpp>
#include <engine/v1/systems/physics.hpp>
#include <engine/v1/systems/scripting.hpp>
#include <engine/v1/components/scripting.hpp>
#include <engine/v1/components/rigidbody.hpp>
#include <engine/v1/components/mesh.hpp>
#include <engine/v1/components/transform.hpp>
#include <engine/v1/world.hpp>
#include <engine/v1/events.hpp>
#include <engine/v1/scripting.hpp>
#include <engine/v1/input.hpp>
#include <engine/v1/audio-manager.hpp>
#include <fstream>

int render();

int main(int argc, char **argv)
{
    Engine::Version version(0, 1, 0);
    std::cout << "Game Engine v" << version.get() << std::endl;

    return render();
}

int render()
{
    Engine::Window window;
    Engine::EventBus events;
    Engine::Input input(window, events);
    Engine::AudioManager audioManager;

    Engine::Camera camera;

    Engine::Primitives::Line grid({-2, 0, 0,
                                   2, 0, 0,
                                   -2, 0, -1,
                                   2, 0, -1,
                                   -2, 0, 1,
                                   2, 0, 1,
                                   -2, 0, -2,
                                   2, 0, -2,
                                   -2, 0, 2,
                                   2, 0, 2,
                                   0, 0, -2,
                                   0, 0, 2,
                                   -1, 0, -2,
                                   -1, 0, 2,
                                   1, 0, -2,
                                   1, 0, 2,
                                   -2, 0, -2,
                                   -2, 0, 2,
                                   2, 0, -2,
                                   2, 0, 2,
                                   -0.5, 0, -2,
                                   -0.5, 0, 2,
                                   0.5, 0, -2,
                                   0.5, 0, 2,
                                   -2, 0, -0.5,
                                   2, 0, -0.5,
                                   -2, 0, 0.5,
                                   2, 0, 0.5,
                                   -1.5, 0, -2,
                                   -1.5, 0, 2,
                                   1.5, 0, -2,
                                   1.5, 0, 2,
                                   -2, 0, -1.5,
                                   2, 0, -1.5,
                                   -2, 0, 1.5,
                                   2, 0, 1.5},
                                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5, .5});

    Engine::World world;
    Engine::Clock clock;

    Engine::ResourceManager rm;

    auto *png2 = static_cast<Engine::Image *>(rm.load("assets/textures/checkerboard-even.png"));

    auto *renderSystem = world.registerSystem<Engine::Systems::RenderSystem>();
    auto *transformSystem = world.registerSystem<Engine::Systems::TransformSystem>();
    auto *physicsSystem = world.registerSystem<Engine::Systems::PhysicsSystem>();
    auto *luaScriptingSystem = world.registerSystem<Engine::Systems::LuaScriptingSystem>(&world);
    auto *wasmScriptingSystem = world.registerSystem<Engine::Systems::WasmScriptingSystem>(&world);

    Engine::Entity *sphereEntity = world.createEntity();
    auto *sphereTransform = sphereEntity->addComponent<Engine::Components::TransformComponent>(glm::vec3(2, 0, 0));
    auto *sphereMesh = sphereEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Sphere>());
    auto *sphereLua = sphereEntity->addComponent<Engine::Components::LuaScriptComponent>();
    luaScriptingSystem->load(sphereLua, "assets/lua/spinner.lua");
    auto *sphereWasm = sphereEntity->addComponent<Engine::Components::WasmScriptComponent>();
    wasmScriptingSystem->load(sphereWasm, "assets/wasm/spinner.wasm");
    sphereMesh->getMesh()->setTexture(*png2);

    float chi = 0.f;
    for (size_t i = 0; i < 5; i++)
    {
        Engine::Entity *cubeEntity = world.createEntity();
        auto *cubeTransform = cubeEntity->addComponent<Engine::Components::TransformComponent>(glm::vec3(-5 * cos(chi), 10 + i * 5, 5 * sin(chi)));
        chi += M_PI / 8;
        cubeEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Cube>());
        auto *cubeRb = cubeEntity->addComponent<Engine::Components::RigidbodyComponent>(glm::vec3(1.f));
        uint32_t cubeHandle = physicsSystem->createBox(cubeTransform->getTransform()->getPosition(), cubeRb->getHalfExtents(), cubeRb->getMass(), cubeRb->getIsStatic());
        cubeRb->setHandle(cubeHandle);
        physicsSystem->setRestitution(cubeHandle, 0.05f);
        physicsSystem->setFriction(cubeHandle, 0.8f);
    }

    // Engine::Entity *cubeEntity = world.createEntity();
    // auto *cubeTransform = cubeEntity->addComponent<Engine::Components::TransformComponent>(glm::vec3(2, 10, 2));
    // cubeEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Cube>());
    // auto *cubeRb = cubeEntity->addComponent<Engine::Components::RigidbodyComponent>(glm::vec3(1.f));
    // uint32_t cubeHandle = physicsSystem->createBox(cubeTransform->getTransform()->getPosition(), cubeRb->getHalfExtents(), cubeRb->getMass(), cubeRb->getIsStatic());
    // cubeRb->setHandle(cubeHandle);
    // physicsSystem->setRestitution(cubeHandle, 0.05f);
    // physicsSystem->setFriction(cubeHandle, 0.8f);

    // Engine::Entity *cube2Entity = world.createEntity();
    // auto *cube2Transform = cube2Entity->addComponent<Engine::Components::TransformComponent>(glm::vec3(3.f, 13, 2.5f));
    // cube2Entity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Cube>());
    // auto *cube2Rb = cube2Entity->addComponent<Engine::Components::RigidbodyComponent>(glm::vec3(1.f));
    // uint32_t cube2Handle = physicsSystem->createBox(cube2Transform->getTransform()->getPosition(), cube2Rb->getHalfExtents(), cube2Rb->getMass(), cube2Rb->getIsStatic());
    // cube2Rb->setHandle(cube2Handle);
    // physicsSystem->setRestitution(cube2Handle, 0.05f);
    // physicsSystem->setFriction(cube2Handle, 0.8f);
    // // physicsSystem->setAngularDamping(cube2Handle, 0.5f);

    Engine::Entity *planeEntity = world.createEntity();
    auto *planeTransform = planeEntity->addComponent<Engine::Components::TransformComponent>(glm::vec3(0, -2, 0));
    planeEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Plane>());
    planeTransform->getTransform()->setScale(glm::vec3(10, 1.f, 10));
    auto *planeRb = planeEntity->addComponent<Engine::Components::RigidbodyComponent>(glm::vec3(10.f, 1.f, 10.f), 0.f, true);
    uint32_t planeHandle = physicsSystem->createBox(planeTransform->getTransform()->getPosition() - glm::vec3(0, 1.f, 0), planeRb->getHalfExtents(), planeRb->getMass(), planeRb->getIsStatic());
    planeRb->setHandle(planeHandle);

    Engine::Entity *gridEntity = world.createEntity();
    auto *gridTransform = gridEntity->addComponent<Engine::Components::TransformComponent>();
    auto *gridMesh = gridEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Sphere>());

    float theta = 0.0001f, phi = M_PI / 4;
    camera.rotate(25.f, {theta, phi});

    audioManager.loadSound("footstep", "assets/audio/footstep-wood.wav");
    audioManager.loadSound("rising", "assets/audio/rising.wav");

    events.subscribe<Engine::ScrollEvent>([&camera, &theta, &phi](const Engine::ScrollEvent &event)
                                          {
                                            theta -= event.getX() / 20;
                                            phi += event.getY() / 20;

                                            if (phi < 0) {
                                                phi = 0.0001f;
                                            }
                                            if (phi > glm::pi<float>()) {
                                                phi = glm::pi<float>() - 0.0001f;
                                            }

                                            camera.rotate(25.f, {theta, phi}); });

    events.subscribe<Engine::KeyEvent>([&window, &camera, &audioManager, gridEntity](const Engine::KeyEvent &event)
                                       {
                                        if (event.getKey() == GLFW_KEY_ESCAPE && event.getAction() == GLFW_PRESS)
                                        {
                                            audioManager.playSound("rising");
                                            window.close();
                                        }
                                        else if(event.getKey() == GLFW_KEY_LEFT && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(-0.5f, 0, 0));
                                            std::ofstream file("grid.json");
                                            nlohmann::json j;
                                            j["grid"] = *gridEntity;
                                            file << j.dump(4);
                                        }
                                        else if(event.getKey() == GLFW_KEY_RIGHT && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0.5f, 0, 0));

                                            std::ifstream file("grid.json");
                                            nlohmann::json j = nlohmann::json::parse(file);
                                            *gridEntity = j.at("grid").get<Engine::Entity>();
                                        }
                                        else if(event.getKey() == GLFW_KEY_UP && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0, 0, -0.5f));
                                        }
                                        else if(event.getKey() == GLFW_KEY_DOWN && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0, 0, 0.5f));
                                        }
                                        else if(event.getKey() == GLFW_KEY_Q && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0,-0.5f,0));
                                        }
                                        else if(event.getKey() == GLFW_KEY_E && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0,0.5f,0));
                                        } });

    while (window.is_open())
    {
        clock.tick();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        auto viewProj = camera.getViewProjection();
        renderSystem->setViewProjection(viewProj);

        world.update(clock.getDeltaTime());
        events.processEvents();
        audioManager.update();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    rm.unload("assets/meshes/sphere.obj");
    rm.unload("assets/textures/checkerboard-even.png");

    return 0;
}