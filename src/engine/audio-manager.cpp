#include <engine/audio-manager.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

struct WAVHeader
{
    char riff[4];
    uint32_t fileSize;
    char wave[4];
    char fmt[4];
    uint32_t fmtSize;
    uint16_t audioFormat;
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4];
    uint32_t dataSize;
};

Engine::AudioManager::AudioManager()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Failed to open audio device" << std::endl;
        return;
    }
    context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context))
    {
        std::cerr << "Failed to create audio context" << std::endl;
        alcCloseDevice(device);
        return;
    }
    alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
}
Engine::AudioManager::~AudioManager()
{
    stopAllSounds();

    for (auto &item : buffers)
    {
        alDeleteBuffers(1, &item.second);
    }

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
void Engine::AudioManager::loadSound(const char *name, const char *filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open audio file: " << filepath << std::endl;
        return;
    }

    // Read RIFF header
    char riff[4];
    uint32_t fileSize;
    char wave[4];

    file.read(riff, 4);
    file.read(reinterpret_cast<char *>(&fileSize), 4);
    file.read(wave, 4);

    if (std::strncmp(riff, "RIFF", 4) != 0 || std::strncmp(wave, "WAVE", 4) != 0)
    {
        std::cerr << "Invalid WAV file: " << filepath << std::endl;
        return;
    }

    // Find fmt chunk
    uint16_t channels, bitsPerSample;
    uint32_t sampleRate;
    char chunkId[4];
    uint32_t chunkSize;

    while (file.read(chunkId, 4))
    {
        file.read(reinterpret_cast<char *>(&chunkSize), 4);

        if (std::strncmp(chunkId, "fmt ", 4) == 0)
        {
            uint16_t audioFormat;
            file.read(reinterpret_cast<char *>(&audioFormat), 2);
            file.read(reinterpret_cast<char *>(&channels), 2);
            file.read(reinterpret_cast<char *>(&sampleRate), 4);

            uint32_t byteRate;
            uint16_t blockAlign;
            file.read(reinterpret_cast<char *>(&byteRate), 4);
            file.read(reinterpret_cast<char *>(&blockAlign), 2);
            file.read(reinterpret_cast<char *>(&bitsPerSample), 2);

            file.seekg(chunkSize - 16, std::ios::cur);
        }
        else if (std::strncmp(chunkId, "data", 4) == 0)
        {
            std::vector<char> audioData(chunkSize);
            file.read(audioData.data(), chunkSize);

            ALuint buffer;
            alGenBuffers(1, &buffer);

            ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
            alBufferData(buffer, format, audioData.data(), audioData.size(), sampleRate);

            buffers[name] = buffer;
            std::cout << "Sound loaded: " << name << " (channels: " << channels << ", sample rate: " << sampleRate << ")" << std::endl;
            break;
        }
        else
        {
            file.seekg(chunkSize, std::ios::cur);
        }
    }
    file.close();
}
void Engine::AudioManager::playSound(const char *name, float volume)
{
    auto item = buffers.find(name);
    if (item == buffers.end())
    {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_GAIN, volume);
    alSourcei(source, AL_BUFFER, item->second);
    alSourcePlay(source);
    sources.push_back(source);

    std::cout << "Playing sound: " << name << " (source: " << source << ")" << std::endl;
}
void Engine::AudioManager::stopAllSounds()
{
    for (auto source : sources)
    {
        alSourceStop(source);
        alDeleteSources(1, &source);
    }
    sources.clear();
}
void Engine::AudioManager::setMasterVolume(float volume)
{
    alListenerf(AL_GAIN, volume);
}
void Engine::AudioManager::update()
{
    std::vector<ALuint> activeSources;
    for (auto source : sources)
    {
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if (state == AL_PLAYING)
        {
            activeSources.push_back(source);
        }
        else
        {
            std::cout << state << std::endl;
            alSourceStop(source);
            alDeleteSources(1, &source);
        }
    }
    sources = std::move(activeSources);
}