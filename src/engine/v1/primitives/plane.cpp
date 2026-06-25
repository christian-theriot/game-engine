/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/primitives/plane.hpp>

Engine::Primitives::Plane::Plane(Image texture, const char *filename)
    : Engine::Mesh(filename, texture)
{
}