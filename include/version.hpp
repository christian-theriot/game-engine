#ifndef __VERSION_HPP
#define __VERSION_HPP

#include <string>

class Version
{
    int major;
    int minor;
    int patch;

public:
    Version(int major, int minor, int patch);

    std::string get() const;
};

#endif