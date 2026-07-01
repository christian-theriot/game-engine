#ifndef __ENGINE_EDITOR_PANEL_SCENE_HPP
#define __ENGINE_EDITOR_PANEL_SCENE_HPP
/*
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/world.hpp>
#include <memory>

namespace Engine::Editor
{
    class ScenePanel
    {
        std::shared_ptr<Scene::World> world;
        int selectedEntityId = -1;
        std::vector<char> newEntityNameBuffer;

        void renderEntityList();
        void renderEntityProperties();
        void renderAddEntityButton();

    public:
        ScenePanel(std::shared_ptr<Scene::World> world);
        void render();
        inline void setWorld(const std::shared_ptr<Scene::World> &newWorld) { world = newWorld; }
    };
}

#endif