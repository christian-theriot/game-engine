#include <engine/v2/scene/systems/audio.hpp>
#include <engine/v2/scene/components/transform.hpp>
#include <engine/v2/scene/world.hpp>
#include <iostream>

Engine::Scene::Systems::Audio::Audio()
{
    device = Core::DeviceHandle(alcOpenDevice(nullptr));
    if (!device)
    {
        std::cerr << "[Audio] Failed to open audio device" << std::endl;
        return;
    }
    context = Core::AudioContextHandle(alcCreateContext(device.get(), nullptr));
    if (!alcMakeContextCurrent(context.get()))
    {
        std::cerr << "[Audio] Failed to create audio context" << std::endl;
        return;
    }
}
Engine::Scene::Systems::Audio::~Audio()
{
    for (auto &[id, source] : sources)
    {
        source.reset();
    }

    context.reset();
    device.reset();
}
void Engine::Scene::Systems::Audio::add(Entity *entity)
{
    auto *audioSource = entity->getComponent<Components::AudioSource>();
    if (!audioSource)
        return;

    ALuint clip = 0;
    if (clips.find(audioSource->getClipPath()) == clips.end())
    {
        auto newClip = Resources::AudioClip::load(audioSource->getClipPath());

        if (!newClip.ok())
        {
            std::cerr << "[Audio] Failed to load audio clip: " << audioSource->getClipPath() << ": " << newClip.error().what() << std::endl;
            return;
        }

        clips[audioSource->getClipPath()] = std::make_unique<Resources::AudioClip>(std::move(newClip.value()));
    }

    clip = clips[audioSource->getClipPath()]->id();

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, clip);
    alSourcef(source, AL_GAIN, audioSource->getVolume());
    alSourcef(source, AL_PITCH, audioSource->getPitch());
    alSourcei(source, AL_LOOPING, audioSource->isLooping() ? AL_TRUE : AL_FALSE);

    sources[entity->getId()] = Core::SourceHandle(source);

    if (audioSource->shouldPlayOnStart())
    {
        alSourcePlay(source);
    }
}
void Engine::Scene::Systems::Audio::remove(Entity *entity)
{
    auto it = sources.find(entity->getId());
    if (it != sources.end())
    {
        it->second.reset();
        sources.erase(it);
    }
}
void Engine::Scene::Systems::Audio::play(Entity *entity)
{
    auto it = sources.find(entity->getId());
    if (it != sources.end())
    {
        alSourcePlay(it->second.get());
    }
}
void Engine::Scene::Systems::Audio::update(World *world, float deltaTime)
{
    for (const auto &entity : world->getEntities())
    {
        auto *transform = entity->getComponent<Components::Transform>();
        auto it = sources.find(entity->getId());

        if (transform && it != sources.end())
        {
            const auto &position = transform->getTransform().position;
            alSource3f(it->second.get(), AL_POSITION, position.x, position.y, position.z);
        }
    }
}
void Engine::Scene::Systems::Audio::setListenerTransform(const Resources::Transform &transform)
{
    const auto &position = transform.position;
    const auto &rotation = transform.rotation;

    alListener3f(AL_POSITION, position.x, position.y, position.z);

    // Convert rotation to forward and up vectors
    glm::vec3 forward = glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    glm::vec3 up = glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));

    float orientation[6] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, orientation);
}