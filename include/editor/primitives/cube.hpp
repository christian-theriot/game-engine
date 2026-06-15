#ifndef __EDITOR_PRIMITIVES_CUBE_HPP
#define __EDITOR_PRIMITIVES_CUBE_HPP

#include <editor/mesh.hpp>

namespace Editor::Primitives
{
    class Cube : public Mesh
    {
    public:
        Cube(Image texture = "assets/textures/checkerboard.png", const char *filename = "assets/meshes/cube.obj");
    };
}

#endif