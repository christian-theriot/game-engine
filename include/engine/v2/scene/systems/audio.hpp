#ifndef __ENGINE_V2_SCENE_SYSTEMS_AUDIO_HPP
#define __ENGINE_V2_SCENE_SYSTEMS_AUDIO_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/resources/audio-clip.hpp>
#include <engine/v2/resources/resource-cache.hpp>
#include <engine/v2/scene/system.hpp>
#include <engine/v2/scene/components/audio-source.hpp>
#include <engine/v2/resources/transform.hpp>
#include <engine/v2/scene/entity.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <unordered_map>

namespace Engine::Scene::Systems
{
    class Audio : public System
    {
        Core::DeviceHandle device;
        Core::AudioContextHandle context;

        std::unordered_map<std::string, std::unique_ptr<Resources::AudioClip>> clips;
        std::unordered_map<uint64_t, Core::SourceHandle> sources;

    public:
        Audio();
        ~Audio();

        void add(Entity *entity);
        void remove(Entity *entity);
        void play(Entity *entity);
        void update(World *world, float deltaTime) override;

        void setListenerTransform(const Resources::Transform &transform);
    };
}

#endif