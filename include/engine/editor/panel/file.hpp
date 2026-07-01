#ifndef __ENGINE_EDITOR_PANEL_FILE_HPP
#define __ENGINE_EDITOR_PANEL_FILE_HPP
/*
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/world.hpp>
#include <engine/events.hpp>
#include <memory>
#include <string>

namespace Engine::Editor
{
    struct FileLoadEvent : public Engine::Event
    {
        std::shared_ptr<Scene::World> world;
    };

    class FilePanel
    {
        std::shared_ptr<Scene::World> world;
        std::string currentFilePath;
        EventBus *events;

    public:
        FilePanel(std::shared_ptr<Scene::World> world, EventBus *events);
        void render();
        inline void setWorld(const std::shared_ptr<Scene::World> &newWorld) { world = newWorld; }
    };
}

#endif