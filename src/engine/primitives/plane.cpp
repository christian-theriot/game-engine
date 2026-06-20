#include <engine/primitives/plane.hpp>

Engine::Primitives::Plane::Plane(Image texture, const char *filename)
    : Engine::Mesh(filename, texture)
{
}