#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <version.hpp>
#include <window.hpp>
#include <camera.hpp>
#include <mesh.hpp>
#include <fstream>
#include <sstream>

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
    Mesh mesh("assets/shaders/default", {-1.0f, -1.0f, 0.0f,
                                         1.0f, -1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f});

    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && window.is_open())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.render(mesh);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}