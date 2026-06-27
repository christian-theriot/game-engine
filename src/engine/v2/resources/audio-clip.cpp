#include <engine/v2/resources/audio-clip.hpp>
#include <fstream>

struct WAVHeader
{
    char riff[4];
    uint32_t fileSize;
    char wave[4];
};

Engine::Core::Result<Engine::Resources::AudioClip> Engine::Resources::AudioClip::load(const std::string &path)
{
    // Load the audio clip from disk and create an OpenAL buffer
    // For simplicity, let's assume we only support WAV files for now

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        return Core::Fail<AudioClip>("Failed to open audio file: " + path);
    }

    WAVHeader header;
    file.read(reinterpret_cast<char *>(&header), sizeof(WAVHeader));

    if (std::strncmp(header.riff, "RIFF", 4) != 0 || std::strncmp(header.wave, "WAVE", 4) != 0)
    {
        return Core::Fail<AudioClip>("Invalid WAV file: " + path);
    }

    // Find fmt chunk
    uint16_t channels, bitsPerSample;
    uint32_t sampleRate;
    char chunkId[4];
    uint32_t chunkSize;

    while (file.read(chunkId, 4))
    {
        file.read(reinterpret_cast<char *>(&chunkSize), sizeof(chunkSize));

        if (std::strncmp(chunkId, "fmt ", 4) == 0)
        {
            uint16_t audioFormat;
            file.read(reinterpret_cast<char *>(&audioFormat), sizeof(audioFormat));
            file.read(reinterpret_cast<char *>(&channels), sizeof(channels));
            file.read(reinterpret_cast<char *>(&sampleRate), sizeof(sampleRate));

            uint32_t byteRate;
            uint16_t blockAlign;
            file.read(reinterpret_cast<char *>(&byteRate), sizeof(byteRate));
            file.read(reinterpret_cast<char *>(&blockAlign), sizeof(blockAlign));
            file.read(reinterpret_cast<char *>(&bitsPerSample), sizeof(bitsPerSample));

            file.seekg(chunkSize - 16, std::ios::cur);
        }
        else if (std::strncmp(chunkId, "data", 4) == 0)
        {
            std::vector<char> audioData(chunkSize);
            file.read(audioData.data(), chunkSize);

            ALuint buffer;
            alGenBuffers(1, &buffer);
            ALenum format = (channels == 1) ? ((bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16)
                                            : ((bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);
            alBufferData(buffer, format, audioData.data(), chunkSize, sampleRate);

            AudioClip clip;
            clip.buffer = Core::AudioBufferHandle(buffer);
            clip.path = path;

            return Core::Result<AudioClip>(std::move(clip));
        }
        else
        {
            // Skip unknown chunk
            file.seekg(chunkSize, std::ios::cur);
        }
    }

    return Core::Fail<AudioClip>("Failed to load audio file: " + path);
}