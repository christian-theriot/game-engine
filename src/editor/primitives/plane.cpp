#include <editor/primitives/plane.hpp>

Editor::Primitives::Plane::Plane(Image texture, const char *filename)
    : Editor::Mesh(filename, texture)
{
}