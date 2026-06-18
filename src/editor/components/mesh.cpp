#include <editor/components/mesh.hpp>

Editor::Components::MeshComponent::MeshComponent(std::unique_ptr<Mesh> mesh)
    : mesh(std::move(mesh))
{
}
Editor::Mesh *Editor::Components::MeshComponent::getMesh() const
{
    return mesh.get();
}
void Editor::Components::MeshComponent::setMesh(std::unique_ptr<Mesh> m)
{
    mesh = std::move(m);
}