#pragma once

#include <Core/Handle.h>

namespace Core {
    typedef uint64_t RenderState;
    typedef Handle<bgfx::ShaderHandle> Shader;
    typedef Handle<bgfx::ShaderHandle> VertexShader;
    typedef Handle<bgfx::ShaderHandle> FragmentShader;
    typedef Handle<bgfx::ProgramHandle> ShaderProgram;
    typedef Handle<bgfx::VertexBufferHandle> VertexBuffer;
    typedef Handle<bgfx::IndexBufferHandle> IndexBuffer;
}
