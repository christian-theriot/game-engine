#ifndef __ENGINE_PRIMITIVES_SPHERE_HPP
#define __ENGINE_PRIMITIVES_SPHERE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/v1/mesh.hpp>

namespace Engine::Primitives
{
    class Sphere : public Mesh
    {
    public:
        Sphere(Image texture = "assets/textures/checkerboard.png", const char *filename = "assets/meshes/sphere.obj");
    };
}

#endif