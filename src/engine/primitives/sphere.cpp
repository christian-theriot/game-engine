#include <engine/primitives/sphere.hpp>

Engine::Primitives::Sphere::Sphere(Image texture, const char *filename)
    : Engine::Mesh(filename, texture)
{
}