#ifndef __ENGINE_RESOURCE_MANAGER_HPP
#define __ENGINE_RESOURCE_MANAGER_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <string>
#include <memory>
#include <unordered_map>

namespace Engine
{
    class IResourceLoader
    {
    public:
        virtual ~IResourceLoader() = default;
        virtual void *load(const std::string &path) = 0;
        virtual void unload(void *resource) = 0;
    };

    class MeshLoader : public IResourceLoader
    {
    public:
        void *load(const std::string &path) override;
        void unload(void *resource) override;
    };

    class ImageLoader : public IResourceLoader
    {
    public:
        void *load(const std::string &path) override;
        void unload(void *resource) override;
    };

    class ResourceManager
    {
        std::unordered_map<std::string, std::unique_ptr<IResourceLoader>> loaders;
        std::unordered_map<std::string, void *> cache;

        std::string getFileExtension(const std::string &path) const;

    public:
        ResourceManager();
        ~ResourceManager();

        void registerLoader(const std::string &extension, std::unique_ptr<IResourceLoader> loader);
        void *load(const std::string &path);
        void unload(const std::string &path);
        void clear();

        bool isCached(const std::string &path) const;
    };
}

#endif