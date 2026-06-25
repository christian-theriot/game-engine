#ifndef __ENGINE_V2_VERSION_HPP
#define __ENGINE_V2_VERSION_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <string>

namespace Engine
{
    class Version
    {
        int major;
        int minor;
        int patch;

    public:
        Version(int major, int minor, int patch);

        std::string get() const;
    };
}

#endif