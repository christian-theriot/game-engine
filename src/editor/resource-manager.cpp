#include <editor/resource-manager.hpp>
#include <editor/mesh.hpp>
#include <editor/image.hpp>
#include <iostream>

void *Editor::MeshLoader::load(const std::string &path)
{
    Mesh *mesh = new Mesh(path.c_str());
    if (!mesh)
    {
        std::cerr << "Failed to load mesh: " << path << std::endl;
    }
    return mesh;
}
void Editor::MeshLoader::unload(void *resource)
{
    delete static_cast<Mesh *>(resource);
}

void *Editor::ImageLoader::load(const std::string &path)
{
    Image *image = new Image(path.c_str());
    if (!image)
    {
        std::cerr << "Failed to load image: " << path << std::endl;
    }
    return image;
}
void Editor::ImageLoader::unload(void *resource)
{
    delete static_cast<Image *>(resource);
}

Editor::ResourceManager::ResourceManager()
{
    registerLoader("obj", std::make_unique<MeshLoader>());
    registerLoader("png", std::make_unique<ImageLoader>());
    registerLoader("bmp", std::make_unique<ImageLoader>());
}
Editor::ResourceManager::~ResourceManager()
{
    clear();
}
void Editor::ResourceManager::registerLoader(const std::string &extension, std::unique_ptr<IResourceLoader> loader)
{
    loaders[extension] = std::move(loader);
}
void *Editor::ResourceManager::load(const std::string &path)
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
void Editor::ResourceManager::unload(const std::string &path)
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
void Editor::ResourceManager::clear()
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
bool Editor::ResourceManager::isCached(const std::string &path) const
{
    return cache.find(path) != cache.end();
}
std::string Editor::ResourceManager::getFileExtension(const std::string &path) const
{
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos)
    {
        return "";
    }
    return path.substr(dotPos + 1);
}