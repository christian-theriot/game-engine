#ifndef __ENGINE_VERSION_HPP
#define __ENGINE_VERSION_HPP

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