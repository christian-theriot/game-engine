#ifndef __EDITOR_COMPONENTS_MESH_HPP
#define __EDITOR_COMPONENTS_MESH_HPP

#include <editor/component.hpp>
#include <editor/mesh.hpp>

namespace Editor::Components
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