/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/events.hpp>

void Engine::EventBus::tick()
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