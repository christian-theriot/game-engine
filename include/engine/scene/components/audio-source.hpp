#ifndef __ENGINE_SCENE_COMPONENTS_AUDIO_SOURCE_HPP
#define __ENGINE_SCENE_COMPONENTS_AUDIO_SOURCE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/component.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace Engine::Scene::Components
{
    class AudioSource : public Component
    {
        std::string clipPath;
        float volume = 1.0f;
        float pitch = 1.0f;
        bool loop = false;
        bool playOnStart = false;

    public:
        AudioSource() = default;
        explicit AudioSource(const std::string &clipPath, float volume = 1.0f, float pitch = 1.0f, bool loop = false, bool playOnStart = false)
            : clipPath(clipPath), volume(volume), pitch(pitch), loop(loop), playOnStart(playOnStart) {}

        inline std::string type() const override { return "AudioSource"; }

        inline const std::string &getClipPath() const { return clipPath; }
        inline void setClipPath(const std::string &path) { clipPath = path; }

        inline float getVolume() const { return volume; }
        inline void setVolume(float vol) { volume = vol; }

        inline float getPitch() const { return pitch; }
        inline void setPitch(float p) { pitch = p; }

        inline bool isLooping() const { return loop; }
        inline void setLooping(bool l) { loop = l; }

        inline bool shouldPlayOnStart() const { return playOnStart; }
        inline void setPlayOnStart(bool play) { playOnStart = play; }
    };

    void to_json(nlohmann::json &j, const AudioSource &component);
    void from_json(const nlohmann::json &j, AudioSource &component);
}

#endif