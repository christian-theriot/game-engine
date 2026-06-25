/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/primitives/cube.hpp>

Engine::Primitives::Cube::Cube(Image texture, const char *filename)
    : Engine::Mesh(filename, texture)
{
}