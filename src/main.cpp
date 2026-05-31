#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <version.hpp>
#include <window.hpp>
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

    Mesh mesh("assets/shaders/default", {-1.0f, -1.0f, 0.0f,
                                         1.0f, -1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f});

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);

    GLfloat x = 4, z = 4, theta = 0.0f;
    glm::mat4 view = glm::lookAt(glm::vec3(x, 3, z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view;

    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && window.is_open())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        x = 4 * cos(theta);
        z = 4 * sin(theta);
        theta += 0.01f;

        view = glm::lookAt(glm::vec3(x, 3, z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        mvp = projection * view;

        mesh.render(mvp);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return 0;
}