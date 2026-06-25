/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/version.hpp>
#include <engine/v2/window.hpp>
#include <engine/v2/events.hpp>
#include <engine/v2/clock.hpp>
#include <engine/v2/resources/mesh.hpp>
#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    Engine::Version version(0, 2, 1);
    Engine::Window window;
    Engine::Clock clock;
    Engine::EventBus events;

    Engine::Resources::Mesh mesh;

    std::cout << "Game Engine v" << version.get() << std::endl;

    while (window.is_open())
    {
        window.glDeclarations();
        clock.tick();

        if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            window.close();
        }

        events.processEvents();
        window.refresh();
    }

    return 0;
}