#ifndef __ENGINE_COMPONENTS_MESH_HPP
#define __ENGINE_COMPONENTS_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/component.hpp>
#include <engine/mesh.hpp>

namespace Engine::Components
{
    class MeshComponent : public Component
    {
        std::unique_ptr<Mesh> mesh;

    public:
        MeshComponent(std::unique_ptr<Mesh> mesh);
        Mesh *getMesh() const;
        void setMesh(std::unique_ptr<Mesh> mesh);
    };
}

#endif