#ifndef __EDITOR_PRIMITIVES_PLANE_HPP
#define __EDITOR_PRIMITIVES_PLANE_HPP

#include <editor/mesh.hpp>

namespace Editor::Primitives
{
    class Plane : public Mesh
    {
    public:
        Plane(Image texture = "assets/textures/checkerboard.png", const char *filename = "assets/meshes/plane.obj");
    };
}

#endif