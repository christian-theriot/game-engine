#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <editor/version.hpp>
#include <editor/window.hpp>
#include <editor/camera.hpp>
#include <editor/image.hpp>
#include <editor/primitives/cube.hpp>
#include <editor/primitives/plane.hpp>
#include <editor/primitives/sphere.hpp>
#include <editor/primitives/line.hpp>
#include <editor/clock.hpp>
#include <editor/resource-manager.hpp>
#include <editor/physics.hpp>
#include <editor/systems/transform.hpp>
#include <editor/systems/render.hpp>
#include <editor/systems/physics.hpp>
#include <editor/components/mesh.hpp>
#include <editor/components/transform.hpp>
#include <editor/components/rigidbody.hpp>
#include <editor/world.hpp>

int render();

int main(int argc, char **argv)
{
    Editor::Version version(1, 0, 0);
    std::cout << "Game Engine v" << version.get() << std::endl;

    return render();
}

int render()
{
    Editor::Window window;

    glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);

    Editor::Camera camera;

    Editor::Primitives::Line grid({-2, 0, 0,
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

    GLfloat theta = 0.f;
    glm::vec3 axis(0, 7.0f, 0.0f);

    Editor::World world;
    Editor::Clock clock;

    Editor::ResourceManager rm;

    auto *png2 = static_cast<Editor::Image *>(rm.load("assets/textures/checkerboard-even.png"));

    auto *renderSystem = world.registerSystem<Editor::Systems::RenderSystem>();
    auto *transformSystem = world.registerSystem<Editor::Systems::TransformSystem>();
    auto *physicsSystem = world.registerSystem<Editor::Systems::PhysicsSystem>();

    Editor::Entity *sphereEntity = world.createEntity();
    auto *sphereTransform = sphereEntity->addComponent<Editor::Components::TransformComponent>(glm::vec3(2, 0, 0));
    auto *sphereMesh = sphereEntity->addComponent<Editor::Components::MeshComponent>(std::make_unique<Editor::Primitives::Sphere>());
    sphereMesh->getMesh()->setTexture(*png2);

    Editor::Entity *cubeEntity = world.createEntity();
    auto *cubeTransform = cubeEntity->addComponent<Editor::Components::TransformComponent>(glm::vec3(2, 0, 2));
    cubeEntity->addComponent<Editor::Components::MeshComponent>(std::make_unique<Editor::Primitives::Cube>());

    Editor::Entity *gridEntity = world.createEntity();
    auto *gridTransform = gridEntity->addComponent<Editor::Components::TransformComponent>();
    gridEntity->addComponent<Editor::Components::MeshComponent>(std::make_unique<Editor::Primitives::Line>(grid));

    auto cubeHull = Editor::Physics::ConvexHull::createBox(glm::vec3(1.f));
    auto *cubePhysics = cubeEntity->addComponent<Editor::Components::RigidbodyComponent>(std::make_unique<Editor::Physics::Rigidbody>(glm::vec3(2, 0, 2), 1.f, cubeHull));

    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && window.is_open())
    {
        clock.tick();
        theta += (M_PI / 4) * clock.getDeltaTime();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        axis.x = 7 * cos(theta);
        axis.y = -7;
        axis.z = 7 * sin(theta);

        camera.rotate(axis);

        auto viewProj = camera.getViewProjection();
        renderSystem->setViewProjection(viewProj);
        world.update(clock.getDeltaTime());

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    rm.unload("assets/meshes/sphere.obj");
    rm.unload("assets/textures/checkerboard-even.png");

    return 0;
}