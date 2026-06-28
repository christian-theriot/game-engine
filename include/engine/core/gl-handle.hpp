#ifndef __ENGINE_CORE_GL_HANDLE_HPP
#define __ENGINE_CORE_GL_HANDLE_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <glad/glad.h>
#include <utility>

namespace Engine::Core
{
    // A move-only RAII wrapper for a single GL handle
    // Deleter is a function pointer chosen at the type level
    template <void (*Deleter)(GLuint)>
    class GLHandle
    {
        GLuint id = 0;

    public:
        inline GLHandle() = default;
        explicit inline GLHandle(GLuint id) : id(id) {}

        inline ~GLHandle() { reset(); }

        // Move-only
        GLHandle(const GLHandle &) = delete;
        GLHandle &operator=(const GLHandle &) = delete;

        inline GLHandle(GLHandle &&other) noexcept : id(std::exchange(other.id, 0)) {}
        inline GLHandle &operator=(GLHandle &&other) noexcept
        {
            if (this != &other)
            {
                reset();
                id = std::exchange(other.id, 0);
            }
            return *this;
        }
        explicit inline operator bool() const noexcept { return id != 0; }
        inline GLuint release() noexcept { return std::exchange(id, 0); }
        inline void reset(GLuint newId = 0) noexcept
        {
            if (id != 0)
            {
                Deleter(id);
            }

            id = newId;
        }
        inline GLuint get() const noexcept { return id; }
    };

    // Deleter adapters (GL signatures vary)
    inline void deleteProgram(GLuint id) { glDeleteProgram(id); }
    inline void deleteShader(GLuint id) { glDeleteShader(id); }
    inline void deleteTexture(GLuint id) { glDeleteTextures(1, &id); }
    inline void deleteGLBuffer(GLuint id) { glDeleteBuffers(1, &id); }
    inline void deleteVertexArray(GLuint id) { glDeleteVertexArrays(1, &id); }

    using ProgramHandle = GLHandle<deleteProgram>;
    using ShaderHandle = GLHandle<deleteShader>;
    using TextureHandle = GLHandle<deleteTexture>;
    using BufferHandle = GLHandle<deleteGLBuffer>;
    using VertexArrayHandle = GLHandle<deleteVertexArray>;
}

#endif