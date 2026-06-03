#include <editor/version.hpp>
#include <sstream>

Version::Version(int major, int minor, int patch)
    : major(major), minor(minor), patch(patch)
{
}

std::string Version::get() const
{
    std::stringstream ss;
    ss << major << "." << minor << "." << patch;
    return ss.str();
}