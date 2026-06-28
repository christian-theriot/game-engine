#ifndef __ENGINE_RENDER_RENDER_MESH_HPP
#define __ENGINE_RENDER_RENDER_MESH_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/core/gl-handle.hpp>
#include <engine/core/result.hpp>
#include <engine/resources/mesh.hpp>
#include <engine/resources/shader.hpp>
#include <engine/resources/texture.hpp>
#include <engine/resources/transform.hpp>
#include <vector>

namespace Engine::Render
{
    class RenderMesh
    {
        Core::VertexArrayHandle VAO;            // Vertex Array Object
        Core::BufferHandle VBO, UVBO, CBO, EBO; // Vertex Buffer Object, UV Buffer Object, Color Buffer Object, Element Array Buffer Object (indices)

        size_t indexCount = 0;
        size_t vertexCount = 0;
        glm::mat4 projection = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);

        inline RenderMesh(Core::VertexArrayHandle vao, Core::BufferHandle vbo, Core::BufferHandle uvbo, Core::BufferHandle cbo, Core::BufferHandle ebo)
            : VAO(std::move(vao)), VBO(std::move(vbo)), UVBO(std::move(uvbo)), CBO(std::move(cbo)), EBO(std::move(ebo)) {}

    public:
        // Create mesh from a file path, or create a cube by default if no path is provided
        // @arg: mesh - the mesh data to load (if already populated and path is empty, will use the provided mesh data)
        // @arg: path - the path to the mesh file to load (if empty, will use the provided mesh data)
        // @return: Result<RenderMesh> - the result of the mesh loading operation, containing either a RenderMesh or an error
        static Core::Result<RenderMesh> load(Resources::Mesh &mesh);

        inline void setProjection(const glm::mat4 &projection) { this->projection = projection; }
        inline void setView(const glm::mat4 &view)
        {
            this->view = view;
        }
        void render(Resources::Transform &transform, const Core::Result<Resources::Shader> &shader, const Core::Result<Resources::Texture> &texture) const;
    };
}

#endif