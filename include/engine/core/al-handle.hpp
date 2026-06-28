#ifndef __ENGINE_CORE_AL_HANDLE_HPP
#define __ENGINE_CORE_AL_HANDLE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <AL/al.h>
#include <AL/alc.h>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace Engine::Core
{
    template <typename T, void (*Deleter)(T)>
    class ALHandle
    {
        T id = (T)0;

    public:
        inline ALHandle() = default;
        explicit inline ALHandle(T id) : id(id) {}

        inline ~ALHandle() { reset(); }

        ALHandle(const ALHandle &) = delete;
        ALHandle &operator=(const ALHandle &) = delete;

        inline ALHandle(ALHandle &&other) noexcept : id(std::exchange(other.id, (T)0)) {}
        inline ALHandle &operator=(ALHandle &&other) noexcept
        {
            if (this != &other)
            {
                reset();
                id = std::exchange(other.id, (T)0);
            }
            return *this;
        }
        explicit inline operator bool() const noexcept { return id != (T)0; }
        inline T release() noexcept { return std::exchange(id, (T)0); }
        inline void reset(T newId = (T)0) noexcept
        {
            if (id != (T)0)
            {
                Deleter(id);
            }

            id = newId;
        }
        inline T get() const noexcept { return id; }
    };

    inline void deleteAudioBuffer(ALuint id) { alDeleteBuffers(1, &id); }
    inline void deleteSource(ALuint id)
    {
        alSourceStop(id);
        alDeleteSources(1, &id);
    }
    inline void deleteAudioContext(ALCcontext *id)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(id);
    }
    inline void deleteDevice(ALCdevice *id) { alcCloseDevice(id); }

    using AudioBufferHandle = ALHandle<ALuint, deleteAudioBuffer>;
    using SourceHandle = ALHandle<ALuint, deleteSource>;
    using AudioContextHandle = ALHandle<ALCcontext *, deleteAudioContext>;
    using DeviceHandle = ALHandle<ALCdevice *, deleteDevice>;
}

#endif