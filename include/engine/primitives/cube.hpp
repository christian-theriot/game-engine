#ifndef __ENGINE_PRIMITIVES_CUBE_HPP
#define __ENGINE_PRIMITIVES_CUBE_HPP

#include <engine/mesh.hpp>

namespace Engine::Primitives
{
    class Cube : public Mesh
    {
    public:
        Cube(Image texture = "assets/textures/checkerboard.png", const char *filename = "assets/meshes/cube.obj");
    };
}

#endif