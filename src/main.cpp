/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/version.hpp>
#include <engine/v2/window.hpp>
#include <engine/v2/events.hpp>
#include <engine/v2/clock.hpp>
#include <engine/v2/resources/mesh.hpp>
#include <engine/v2/resources/transform.hpp>
#include <engine/v2/resources/shader.hpp>
#include <engine/v2/resources/texture.hpp>
#include <engine/v2/render/render-mesh.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    Engine::Version version(0, 2, 3);
    Engine::Window window;
    Engine::Clock clock;
    Engine::EventBus events;

    std::cout << "Game Engine v" << version.get() << std::endl;

    auto projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
    auto view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    Engine::Resources::Transform transform;
    auto shader = Engine::Resources::Shader::load("texture");
    auto texture = Engine::Resources::Texture::load("checkerboard-even.png");
    Engine::Resources::Mesh mesh;
    auto renderMesh = Engine::Render::RenderMesh::load(mesh, "cube.obj");

    if (!renderMesh.ok())
    {
        std::cout << "Failed to load mesh: " << renderMesh.error().what() << std::endl;
        return -1;
    }

    renderMesh.value().setProjection(projection);
    renderMesh.value().setView(view);

    while (window.is_open())
    {
        window.glDeclarations();
        clock.tick();

        if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window.close();
        }

        renderMesh.value().render(transform, shader, texture);

        events.processEvents();
        window.refresh();
    }

    return 0;
}