#include <editor/clock.hpp>

Editor::Clock::Clock()
    : deltaTime(0.f), totalElapsedTime(0.f), paused(false), timeScale(1.f)
{
    start();
}

void Editor::Clock::start()
{
    startTime = std::chrono::high_resolution_clock::now();
    lastFrameTime = startTime;
    totalElapsedTime = 0.f;
    deltaTime = 0.f;
}
void Editor::Clock::tick()
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
void Editor::Clock::pause()
{
    paused = true;
    deltaTime = 0.f;
}
void Editor::Clock::resume()
{
    paused = false;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}
void Editor::Clock::reset()
{
    start();
}
float Editor::Clock::getDeltaTime() const
{
    return deltaTime;
}
float Editor::Clock::getElapsedTime() const
{
    return totalElapsedTime;
}
float Editor::Clock::getFPS() const
{
    return fps;
}
float Editor::Clock::getTimeScale() const
{
    return timeScale;
}
void Editor::Clock::setTimeScale(float timeScale)
{
    if (-0.00001f <= timeScale && timeScale <= 0.00001f)
    {
        timeScale = 0.0f;
    }

    this->timeScale = timeScale;
}
bool Editor::Clock::isPaused() const
{
    return paused;
}
void Editor::Clock::setMaxDeltaTime(float max)
{
    deltaTime = std::min(deltaTime, max);
}