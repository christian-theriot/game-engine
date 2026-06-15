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

    Editor::Image png1("assets/textures/checkerboard-even.png");
    Editor::Image png2("assets/textures/checkerboard-w-color-even.png");
    Editor::Image bmp1("assets/textures/sand.bmp");

    Editor::Primitives::Sphere sphere(png1, "assets/meshes/plane.obj");
    Editor::Primitives::Line xAxis({0, 0, 0, 1, 0, 0}, {1, 0, 0, 1, 0, 0});
    Editor::Primitives::Line yAxis({0, 0, 0, 0, 1, 0}, {0, 1, 0, 0, 1, 0});
    Editor::Primitives::Line zAxis({0, 0, 0, 0, 0, 1}, {0, 0, 1, 0, 0, 1});
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

    GLfloat theta = 0.0f, phi = M_PI_4;
    glm::vec3 axis(0, 7.0f, 0.0f);
    GLfloat zoom = 7;

    double lastX, lastY, deltaX, deltaY;
    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && window.is_open())
    {
        double xpos, ypos;
        glfwGetCursorPos(window.get(), &xpos, &ypos);
        deltaX = xpos - lastX;
        deltaY = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        if (glfwGetKey(window.get(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            if (abs(deltaX) > 0.001)
            {
                theta += deltaX * 0.005;
            }

            if (abs(deltaY) > 0.001)
            {
                phi -= deltaY * 0.01;

                if (phi <= 0.0)
                {
                    phi = 0.001;
                }

                if (phi >= M_PI - 0.001f)
                {
                    phi = M_PI - 0.001f;
                }
            }
        }

        if (glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS)
        {
            zoom -= 0.05f;
            if (zoom <= 3.f)
            {
                zoom = 3.f;
            }
        }
        if (glfwGetKey(window.get(), GLFW_KEY_E) == GLFW_PRESS)
        {
            zoom += 0.05f;
            if (zoom >= 10.f)
            {
                zoom = 10.f;
            }
        }

        axis.x = zoom * sin(phi) * cos(theta);
        axis.y = zoom * cos(phi);
        axis.z = zoom * sin(phi) * sin(theta);
        camera.rotate(axis);

        camera.render(grid);
        camera.render(sphere);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}