#ifndef __ENGINE_EVENTS_HPP
#define __ENGINE_EVENTS_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <memory>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Engine
{
    struct Event
    {
        virtual ~Event() = default;
    };

    class EventBus
    {
        struct DeferredEvent
        {
            std::type_index type;
            std::shared_ptr<Event> event;
        };

        std::vector<DeferredEvent> currentQueue;
        std::vector<DeferredEvent> nextQueue;
        std::unordered_map<size_t, std::vector<std::function<void(const Event &)>>> handlers;

    public:
        template <typename EventType>
        void subscribe(std::function<void(const EventType &)> handler)
        {
            static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");

            auto wrapper = [handler](const Event &event)
            {
                handler(static_cast<const EventType &>(event));
            };
            handlers[typeid(EventType).hash_code()].push_back(wrapper);
        }

        template <typename EventType>
        void publish(const EventType &event)
        {
            static_assert(std::is_base_of_v<Event, EventType>, "EventType must derive from Event");
            nextQueue.push_back({std::type_index(typeid(EventType)), std::make_shared<EventType>(event)});
        }

        void processEvents();
    };
}

#endif