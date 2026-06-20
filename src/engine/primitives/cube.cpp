#include <engine/primitives/cube.hpp>

Engine::Primitives::Cube::Cube(Image texture, const char *filename)
    : Engine::Mesh(filename, texture)
{
}