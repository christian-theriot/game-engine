#ifndef __ENGINE_RESOURCES_AUDIO_CLIP_HPP
#define __ENGINE_RESOURCES_AUDIO_CLIP_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/core/al-handle.hpp>
#include <engine/core/result.hpp>
#include <string>
#include <nlohmann/json.hpp>

namespace Engine::Resources
{
    class AudioClip
    {
        Core::AudioBufferHandle buffer;
        std::string path;

    public:
        static Core::Result<AudioClip> load(const std::string &path);

        inline ALuint id() const { return buffer.get(); }
        inline const std::string &getPath() const { return path; }
    };

    void to_json(nlohmann::json &j, const AudioClip &clip);
    void from_json(const nlohmann::json &j, AudioClip &clip);
}

#endif