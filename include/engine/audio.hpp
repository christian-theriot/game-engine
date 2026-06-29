#ifndef __ENGINE_AUDIO_HPP
#define __ENGINE_AUDIO_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/resources/audio-clip.hpp>
#include <engine/resources/resource-cache.hpp>
#include <engine/scene/components/audio-source.hpp>
#include <engine/resources/transform.hpp>
#include <engine/scene/entity.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <unordered_map>

namespace Engine
{
    namespace Scene
    {
        class World;
    }

    class Audio
    {
        Core::DeviceHandle device;
        Core::AudioContextHandle context;

        std::unordered_map<std::string, std::unique_ptr<Resources::AudioClip>> clips;
        std::unordered_map<uint64_t, Core::SourceHandle> sources;

    public:
        Audio();
        ~Audio();

        void add(Scene::Entity *entity);
        void remove(Scene::Entity *entity);
        void play(Scene::Entity *entity);
        void update(Scene::World *world, float deltaTime);

        void setListenerTransform(const Resources::Transform &transform);
    };
}

#endif