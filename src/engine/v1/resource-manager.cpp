/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/resource-manager.hpp>
#include <engine/v1/mesh.hpp>
#include <engine/v1/image.hpp>
#include <iostream>

void *Engine::MeshLoader::load(const std::string &path)
{
    Mesh *mesh = new Mesh(path.c_str());
    if (!mesh)
    {
        std::cerr << "Failed to load mesh: " << path << std::endl;
    }
    return mesh;
}
void Engine::MeshLoader::unload(void *resource)
{
    delete static_cast<Mesh *>(resource);
}

void *Engine::ImageLoader::load(const std::string &path)
{
    Image *image = new Image(path.c_str());
    if (!image)
    {
        std::cerr << "Failed to load image: " << path << std::endl;
    }
    return image;
}
void Engine::ImageLoader::unload(void *resource)
{
    delete static_cast<Image *>(resource);
}

Engine::ResourceManager::ResourceManager()
{
    registerLoader("obj", std::make_unique<MeshLoader>());
    registerLoader("png", std::make_unique<ImageLoader>());
    registerLoader("bmp", std::make_unique<ImageLoader>());
}
Engine::ResourceManager::~ResourceManager()
{
    clear();
}
void Engine::ResourceManager::registerLoader(const std::string &extension, std::unique_ptr<IResourceLoader> loader)
{
    loaders[extension] = std::move(loader);
}
void *Engine::ResourceManager::load(const std::string &path)
{
    if (isCached(path))
    {
        return cache[path];
    }

    std::string ext = getFileExtension(path);

    auto item = loaders.find(ext);
    if (item == loaders.end())
    {
        std::cerr << "No loader registered for extension: " << ext << std::endl;
        return nullptr;
    }

    void *resource = item->second->load(path);
    if (resource)
    {
        cache[path] = resource;
    }

    return resource;
}
void Engine::ResourceManager::unload(const std::string &path)
{
    auto item = cache.find(path);
    if (item != cache.end())
    {
        std::string ext = getFileExtension(path);
        auto loader = loaders.find(ext);

        if (loader != loaders.end())
        {
            loader->second->unload(item->second);
        }

        cache.erase(item);
    }
}
void Engine::ResourceManager::clear()
{
    for (auto &item : cache)
    {
        std::string ext = getFileExtension(item.first);
        auto loader = loaders.find(ext);

        if (loader != loaders.end())
        {
            loader->second->unload(item.second);
        }
    }
    cache.clear();
}
bool Engine::ResourceManager::isCached(const std::string &path) const
{
    return cache.find(path) != cache.end();
}
std::string Engine::ResourceManager::getFileExtension(const std::string &path) const
{
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos)
    {
        return "";
    }
    return path.substr(dotPos + 1);
}