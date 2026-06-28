/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/scene/systems/clock.hpp>

Engine::Scene::Systems::Clock::Clock()
    : deltaTime(0.f), totalElapsedTime(0.f), paused(false), timeScale(1.f)
{
    start();
}

void Engine::Scene::Systems::Clock::start()
{
    startTime = std::chrono::high_resolution_clock::now();
    lastFrameTime = startTime;
    totalElapsedTime = 0.f;
    deltaTime = 0.f;
}
void Engine::Scene::Systems::Clock::tick()
{
    if (paused)
    {
        deltaTime = 0.f;
        return;
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastFrameTime;

    float effectiveTimeScale = (-0.00001f <= timeScale && timeScale <= 0.00001f) ? 1.f : timeScale;
    deltaTime = elapsed.count() * effectiveTimeScale;

    lastFrameTime = currentTime;
    totalElapsedTime += deltaTime;

    float newFps = (-0.00001f <= deltaTime && deltaTime <= 0.00001f) ? 0.f : (1.f / std::fabs(deltaTime));
    fps = (fps + newFps) / 2;
}
void Engine::Scene::Systems::Clock::pause()
{
    paused = true;
    deltaTime = 0.f;
}
void Engine::Scene::Systems::Clock::resume()
{
    paused = false;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}
void Engine::Scene::Systems::Clock::reset()
{
    start();
}
float Engine::Scene::Systems::Clock::getDeltaTime() const
{
    return deltaTime;
}
float Engine::Scene::Systems::Clock::getElapsedTime() const
{
    return totalElapsedTime;
}
float Engine::Scene::Systems::Clock::getFPS() const
{
    return fps;
}
float Engine::Scene::Systems::Clock::getTimeScale() const
{
    return timeScale;
}
void Engine::Scene::Systems::Clock::setTimeScale(float timeScale)
{
    if (-0.00001f <= timeScale && timeScale <= 0.00001f)
    {
        timeScale = 0.0f;
    }

    this->timeScale = timeScale;
}
bool Engine::Scene::Systems::Clock::isPaused() const
{
    return paused;
}
void Engine::Scene::Systems::Clock::setMaxDeltaTime(float max)
{
    deltaTime = std::min(deltaTime, max);
}