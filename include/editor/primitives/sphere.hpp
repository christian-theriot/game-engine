#ifndef __EDITOR_PRIMITIVES_SPHERE_HPP
#define __EDITOR_PRIMITIVES_SPHERE_HPP

#include <editor/mesh.hpp>

namespace Editor::Primitives
{
    class Sphere : public Mesh
    {
    public:
        Sphere(Image texture = "assets/textures/checkerboard.png", const char *filename = "assets/meshes/sphere.obj");
    };
}

#endif