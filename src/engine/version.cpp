/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/version.hpp>
#include <sstream>

Engine::Version::Version(int major, int minor, int patch)
    : major(major), minor(minor), patch(patch)
{
}

std::string Engine::Version::get() const
{
    std::stringstream ss;
    ss << major << "." << minor << "." << patch;
    return ss.str();
}