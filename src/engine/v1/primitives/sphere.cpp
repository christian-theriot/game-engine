/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/primitives/sphere.hpp>

Engine::Primitives::Sphere::Sphere(Image texture, const char *filename)
    : Engine::Mesh(filename, texture)
{
}