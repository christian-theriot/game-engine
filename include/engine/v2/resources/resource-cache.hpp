#ifndef __ENGINE_V2_RESOURCES_RESOURCE_CACHE_HPP
#define __ENGINE_V2_RESOURCES_RESOURCE_CACHE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <memory>
#include <string>
#include <unordered_map>

namespace Engine::Resources
{
    // Caches shared, immutable GPU resources keyed by their source path.
    // Loading is deferred to a user-supplied loader
    template <typename T>
    class ResourceCache
    {
        std::unordered_map<std::string, std::shared_ptr<T>> cache;

    public:
        template <typename Loader>
        inline std::shared_ptr<T> get(const std::string &key, Loader &&loader)
        {
            auto item = cache.find(key);
            if (item != cache.end())
            {
                return item->second;
            }

            auto resource = std::forward<Loader>(loader)(key);
            cache[key] = resource;
            return resource;
        }

        inline void clear() { cache.clear(); }
        inline size_t size() const { return cache.size(); }
    };
}

#endif