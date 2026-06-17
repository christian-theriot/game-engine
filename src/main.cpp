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

    Editor::Clock clock;

    Editor::ResourceManager rm;

    auto *sphere = static_cast<Editor::Mesh *>(rm.load("assets/meshes/sphere.obj"));
    auto *png2 = static_cast<Editor::Image *>(rm.load("assets/textures/checkerboard-even.png"));
    auto *secondSphere = static_cast<Editor::Mesh *>(rm.load("assets/meshes/sphere.obj"));

    assert(sphere == secondSphere);

    sphere->setTexture(*png2);

    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && window.is_open())
    {
        clock.tick();
        float FPS = clock.getFPS();
        std::cout << "FPS: " << FPS << std::endl;

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

        camera.render(grid);
        // camera.render(map);
        camera.render(*sphere);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    rm.unload("assets/meshes/sphere.obj");
    rm.unload("assets/textures/checkerboard-even.png");

    return 0;
}