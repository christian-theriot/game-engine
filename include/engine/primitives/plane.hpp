#ifndef __ENGINE_PRIMITIVES_PLANE_HPP
#define __ENGINE_PRIMITIVES_PLANE_HPP

#include <engine/mesh.hpp>

namespace Engine::Primitives
{
    class Plane : public Mesh
    {
    public:
        Plane(Image texture = "assets/textures/checkerboard.png", const char *filename = "assets/meshes/plane.obj");
    };
}

#endif