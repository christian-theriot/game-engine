#ifndef __ENGINE_AUDIO_MANAGER_HPP
#define __ENGINE_AUDIO_MANAGER_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace Engine
{
    class AudioManager
    {
        ALCdevice *device;
        ALCcontext *context;
        std::unordered_map<std::string, ALuint> buffers;
        std::vector<ALuint> sources;

    public:
        AudioManager();
        ~AudioManager();

        void loadSound(const char *name, const char *filepath);

        void playSound(const char *name, float volume = 1.f);
        void stopAllSounds();

        void setMasterVolume(float volume);

        void update();
    };
}

#endif