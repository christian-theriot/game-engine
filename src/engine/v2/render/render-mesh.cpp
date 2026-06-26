#include <engine/v2/render/render-mesh.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include <iostream>

Engine::Core::Result<Engine::Render::RenderMesh> Engine::Render::RenderMesh::load(Resources::Mesh &mesh)
{
    if (!mesh.path.empty())
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile("assets/meshes/" + mesh.path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

        if (!scene || !scene->HasMeshes())
        {
            return Core::Fail<RenderMesh>("Assimp error: " + std::string(importer.GetErrorString()));
        }

        // TODO: Handle multiple meshes in the scene, for now we just take the first one
        const aiMesh *ai_mesh = scene->mMeshes[0];

        // Empty the mesh data before loading new data
        mesh.vertices.clear();
        mesh.uvs.clear();
        mesh.colors.clear();
        mesh.indices.clear();

        for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
        {
            const aiVector3D &pos = ai_mesh->mVertices[i];
            const aiVector3D &uv = ai_mesh->HasTextureCoords(0) ? ai_mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
            const aiColor4D &color = ai_mesh->HasVertexColors(0) ? ai_mesh->mColors[0][i] : aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);

            mesh.vertices.push_back(pos.x);
            mesh.vertices.push_back(pos.y);
            mesh.vertices.push_back(pos.z);

            mesh.uvs.push_back(uv.x);
            mesh.uvs.push_back(uv.y);

            mesh.colors.push_back(color.r);
            mesh.colors.push_back(color.g);
            mesh.colors.push_back(color.b);
            mesh.colors.push_back(color.a);
        }

        for (unsigned f = 0; f < ai_mesh->mNumFaces; ++f)
        {
            const aiFace &face = ai_mesh->mFaces[f];
            for (unsigned j = 0; j < face.mNumIndices; ++j)
            {
                mesh.indices.push_back(face.mIndices[j]);
            }
        }
    }

    // Create VAO and VBOs
    GLuint vao = 0, vbo = 0, uvbo = 0, cbo = 0, ebo = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), mesh.vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &uvbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.uvs.size() * sizeof(GLfloat), mesh.uvs.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.colors.size() * sizeof(GLfloat), mesh.colors.data(), GL_STATIC_DRAW);

    if (!mesh.indices.empty())
    {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

    if (!mesh.indices.empty())
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    auto renderMesh = RenderMesh(Core::VertexArrayHandle(vao), Core::BufferHandle(vbo), Core::BufferHandle(uvbo), Core::BufferHandle(cbo), Core::BufferHandle(ebo));
    renderMesh.indexCount = mesh.indices.size();
    renderMesh.vertexCount = mesh.vertices.size() / 3;

    return Core::Ok(std::move(renderMesh));
}
void Engine::Render::RenderMesh::render(const Resources::Transform &transform, const Core::Result<Resources::Shader> &shader, const Core::Result<Resources::Texture> &texture) const
{
    if (shader.ok())
    {
        shader.value().use();
        GLint matrixID = glGetUniformLocation(shader.value().id(), "MVP");

        if (matrixID == -1)
        {
            std::cerr << "Warning: MVP uniform not found in shader." << std::endl;
            return;
        }

        glm::mat4 mvp = projection * view * transform.updated();
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
    }
    else
    {
        std::cerr << "Warning: Shader is not valid. Cannot render mesh." << std::endl;
        return;
    }

    if (texture.ok())
    {
        texture.value().use();
    }
    else
    {
        std::cerr << "Warning: Texture is not valid. Rendering without texture." << std::endl;
    }

    glBindVertexArray(VAO.get());

    if (indexCount)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
    }

    glBindVertexArray(0);
}