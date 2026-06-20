#include <engine/events.hpp>

void Engine::EventBus::processEvents()
{
    currentQueue = std::move(nextQueue);

    for (auto &queuedEvent : currentQueue)
    {
        auto item = handlers.find(queuedEvent.type.hash_code());
        if (item != handlers.end())
        {
            for (auto &handler : item->second)
            {
                handler(*queuedEvent.event);
            }
        }
    }

    currentQueue.clear();
}