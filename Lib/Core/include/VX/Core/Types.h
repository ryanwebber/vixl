#pragma once

#include <VX/Core/Handle.h>

namespace VX::Core {
    typedef uint64_t RenderState;
    typedef Handle<bgfx::ShaderHandle> ShaderHandle;
    typedef Handle<bgfx::ProgramHandle> ProgramHandle;
    typedef Handle<bgfx::TextureHandle> TextureHandle;
    typedef Handle<bgfx::VertexBufferHandle> VertexBufferHandle;
    typedef Handle<bgfx::IndexBufferHandle> IndexBufferHandle;
}
