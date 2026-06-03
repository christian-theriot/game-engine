#ifndef __EDITOR_VERSION_HPP
#define __EDITOR_VERSION_HPP

#include <string>

namespace Editor
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