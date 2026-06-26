#ifndef __ENGINE_V2_SCENE_SYSTEMS_CLOCK_HPP
#define __ENGINE_V2_SCENE_SYSTEMS_CLOCK_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/scene/system.hpp>
#include <chrono>

namespace Engine::Scene::Systems
{
    class Clock : public Scene::System
    {
    private:
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::high_resolution_clock::time_point lastFrameTime;
        float deltaTime;
        float totalElapsedTime;
        bool paused;
        float fps;
        float timeScale;

    public:
        Clock();

        // Core timing methods
        void start();
        void tick();
        void pause();
        void resume();
        void reset();

        inline void update(Scene::World *world, float deltaTime) override { tick(); }

        // Getters
        float getDeltaTime() const;   // Time since last frame (seconds)
        float getElapsedTime() const; // Total time since start (seconds)
        float getFPS() const;         // Frames per second
        float getTimeScale() const;

        // Setter
        void setTimeScale(float timeScale);

        // Utilities
        bool isPaused() const;
        void setMaxDeltaTime(float max); // Prevent large jumps (useful for debugging)
    };
}

#endif