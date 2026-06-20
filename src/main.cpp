#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/version.hpp>
#include <engine/window.hpp>
#include <engine/camera.hpp>
#include <engine/image.hpp>
#include <engine/primitives/cube.hpp>
#include <engine/primitives/plane.hpp>
#include <engine/primitives/sphere.hpp>
#include <engine/primitives/line.hpp>
#include <engine/clock.hpp>
#include <engine/resource-manager.hpp>
#include <engine/physics.hpp>
#include <engine/systems/transform.hpp>
#include <engine/systems/render.hpp>
#include <engine/systems/physics.hpp>
#include <engine/components/mesh.hpp>
#include <engine/components/transform.hpp>
#include <engine/components/rigidbody.hpp>
#include <engine/world.hpp>
#include <engine/events.hpp>
#include <engine/input.hpp>
#include <engine/audio-manager.hpp>

int render();

int main(int argc, char **argv)
{
    Engine::Version version(1, 0, 0);
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

    Engine::Entity *sphereEntity = world.createEntity();
    auto *sphereTransform = sphereEntity->addComponent<Engine::Components::TransformComponent>(glm::vec3(2, 0, 0));
    auto *sphereMesh = sphereEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Sphere>());
    sphereMesh->getMesh()->setTexture(*png2);

    Engine::Entity *cubeEntity = world.createEntity();
    auto *cubeTransform = cubeEntity->addComponent<Engine::Components::TransformComponent>(glm::vec3(2, 0, 2));
    cubeEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Cube>());

    Engine::Entity *gridEntity = world.createEntity();
    auto *gridTransform = gridEntity->addComponent<Engine::Components::TransformComponent>();
    gridEntity->addComponent<Engine::Components::MeshComponent>(std::make_unique<Engine::Primitives::Line>(grid));

    auto cubeHull = Engine::Physics::ConvexHull::createBox(glm::vec3(1.f));
    auto *cubePhysics = cubeEntity->addComponent<Engine::Components::RigidbodyComponent>(std::make_unique<Engine::Physics::Rigidbody>(glm::vec3(2, 0, 2), 1.f, cubeHull));

    float theta = 0.0001f, phi = M_PI / 4;
    camera.rotate(7.f, {theta, phi});

    audioManager.loadSound("footstep", "assets/audio/footstep-wood.wav");

    events.subscribe<Engine::ScrollEvent>([&camera, &theta, &phi](const Engine::ScrollEvent &event)
                                          {
                                            theta += event.getX() / 20;
                                            phi += event.getY() / 20;

                                            if (phi < 0) {
                                                phi = 0.0001f;
                                            }
                                            if (phi > glm::pi<float>()) {
                                                phi = glm::pi<float>() - 0.0001f;
                                            }

                                            camera.rotate(7.f, {theta, phi}); });

    events.subscribe<Engine::KeyEvent>([&window, &camera, &audioManager](const Engine::KeyEvent &event)
                                       {
                                        if (event.getKey() == GLFW_KEY_ESCAPE && event.getAction() == GLFW_PRESS)
                                        {
                                            window.close();
                                        }
                                        else if(event.getKey() == GLFW_KEY_LEFT && event.getAction() != GLFW_RELEASE) {
                                            audioManager.playSound("footstep");
                                            camera.translate(glm::vec3(-0.5f, 0, 0));
                                        }
                                        else if(event.getKey() == GLFW_KEY_RIGHT && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0.5f, 0, 0));
                                        }
                                        else if(event.getKey() == GLFW_KEY_UP && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0, 0, -0.5f));
                                        }
                                        else if(event.getKey() == GLFW_KEY_DOWN && event.getAction() != GLFW_RELEASE) {
                                            camera.translate(glm::vec3(0, 0, 0.5f));
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