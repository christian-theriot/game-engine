#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <editor/version.hpp>
#include <editor/window.hpp>
#include <editor/camera.hpp>
#include <editor/mesh.hpp>
#include <editor/image.hpp>
#include <editor/mesh-w-color.hpp>
#include <editor/mesh-w-tex.hpp>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>

int render();

int main(int argc, char **argv)
{
    Version version(1, 0, 0);
    std::cout << "Game Engine v" << version.get() << std::endl;

    return render();
}

int render()
{
    Window window;

    glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);

    Camera camera;

    // BMP bmp1("assets/textures/sand.bmp");
    Image png1("assets/textures/checkerboard.png");

    MeshWithTexture mesh1("assets/shaders/texture",
                          {-1.0f, -1.0f, -1.0f, // triangle 1 : begin
                           -1.0f, -1.0f, 1.0f,
                           -1.0f, 1.0f, 1.0f, // triangle 1 : end
                           1.0f, 1.0f, -1.0f, // triangle 2 : begin
                           -1.0f, -1.0f, -1.0f,
                           -1.0f, 1.0f, -1.0f, // triangle 2 : end
                           1.0f, -1.0f, 1.0f,
                           -1.0f, -1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, 1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           -1.0f, -1.0f, -1.0f,
                           -1.0f, -1.0f, -1.0f,
                           -1.0f, 1.0f, 1.0f,
                           -1.0f, 1.0f, -1.0f,
                           1.0f, -1.0f, 1.0f,
                           -1.0f, -1.0f, 1.0f,
                           -1.0f, -1.0f, -1.0f,
                           -1.0f, 1.0f, 1.0f,
                           -1.0f, -1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, 1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, 1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, -1.0f,
                           -1.0f, 1.0f, -1.0f,
                           1.0f, 1.0f, 1.0f,
                           -1.0f, 1.0f, -1.0f,
                           -1.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f,
                           -1.0f, 1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f},
                          {0.f, 1.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f});

    Image png2("assets/textures/checkerboard-w-color.png");
    MeshWithTexture mesh2("assets/shaders/texture",
                          {1.0f, -1.0f, -1.0f, // triangle 1 : begin
                           1.0f, -1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f,  // triangle 1 : end
                           3.0f, 1.0f, -1.0f, // triangle 2 : begin
                           1.0f, -1.0f, -1.0f,
                           1.0f, 1.0f, -1.0f, // triangle 2 : end
                           3.0f, -1.0f, 1.0f,
                           1.0f, -1.0f, -1.0f,
                           3.0f, -1.0f, -1.0f,
                           3.0f, 1.0f, -1.0f,
                           3.0f, -1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, -1.0f,
                           3.0f, -1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f,
                           1.0f, -1.0f, -1.0f,
                           1.0f, 1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f,
                           3.0f, -1.0f, 1.0f,
                           3.0f, 1.0f, 1.0f,
                           3.0f, -1.0f, -1.0f,
                           3.0f, 1.0f, -1.0f,
                           3.0f, -1.0f, -1.0f,
                           3.0f, 1.0f, 1.0f,
                           3.0f, -1.0f, 1.0f,
                           3.0f, 1.0f, 1.0f,
                           3.0f, 1.0f, -1.0f,
                           1.0f, 1.0f, -1.0f,
                           3.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, -1.0f,
                           1.0f, 1.0f, 1.0f,
                           3.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f,
                           3.0f, -1.0f, 1.0f},
                          {0.f, 1.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f});

    Image bmp1("assets/textures/sand.bmp");
    MeshWithTexture mesh3("assets/shaders/texture",
                          {1.0f, -1.0f, 1.0f, // triangle 1 : begin
                           1.0f, -1.0f, 3.0f,
                           1.0f, 1.0f, 3.0f, // triangle 1 : end
                           3.0f, 1.0f, 1.0f, // triangle 2 : begin
                           1.0f, -1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f, // triangle 2 : end
                           3.0f, -1.0f, 3.0f,
                           1.0f, -1.0f, 1.0f,
                           3.0f, -1.0f, 1.0f,
                           3.0f, 1.0f, 1.0f,
                           3.0f, -1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f,
                           1.0f, -1.0f, 1.0f,
                           1.0f, 1.0f, 3.0f,
                           1.0f, 1.0f, 1.0f,
                           3.0f, -1.0f, 3.0f,
                           1.0f, -1.0f, 3.0f,
                           1.0f, -1.0f, 1.0f,
                           1.0f, 1.0f, 3.0f,
                           1.0f, -1.0f, 3.0f,
                           3.0f, -1.0f, 3.0f,
                           3.0f, 1.0f, 3.0f,
                           3.0f, -1.0f, 1.0f,
                           3.0f, 1.0f, 1.0f,
                           3.0f, -1.0f, 1.0f,
                           3.0f, 1.0f, 3.0f,
                           3.0f, -1.0f, 3.0f,
                           3.0f, 1.0f, 3.0f,
                           3.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, 1.0f,
                           3.0f, 1.0f, 3.0f,
                           1.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, 3.0f,
                           3.0f, 1.0f, 3.0f,
                           1.0f, 1.0f, 3.0f,
                           3.0f, -1.0f, 3.0f},
                          {0.f, 1.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 1.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           0.f, 0.f,
                           0.f, 1.f,
                           1.f, 1.f,
                           0.f, 0.f,
                           1.f, 1.f,
                           1.f, 0.f,
                           1.f, 0.f,
                           0.f, 0.f,
                           1.f, 1.f});

    GLfloat theta = 0.0f, phi = 0.0f;
    glm::vec3 axis(0, 7.0f, 0.0f);
    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && window.is_open())
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera.render(mesh);

        if (glfwGetKey(window.get(), GLFW_KEY_UP) == GLFW_PRESS)
            phi += 0.01f;
        if (glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_PRESS)
            phi -= 0.01f;
        if (glfwGetKey(window.get(), GLFW_KEY_LEFT) == GLFW_PRESS)
            theta -= 0.01f;
        if (glfwGetKey(window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS)
            theta += 0.01f;

        axis.x = 7 * cos(theta);
        axis.y = 7 * cos(phi);
        axis.z = 7 * sin(theta);
        camera.rotate(axis);

        png1.use();
        camera.render(mesh1);

        png2.use();
        camera.render(mesh2);

        bmp1.use();
        camera.render(mesh3);
        // camera.render(mesh2);
        // camera.render(mesh3);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}