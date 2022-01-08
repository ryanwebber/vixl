#pragma once

#include <memory>
#include <bgfx/bgfx.h>
#include <Core/Material.h>
#include <Core/Handle.h>
#include <Core/Types.h>

namespace Core {
    struct RenderCommand {
        RenderState state { BGFX_STATE_DEFAULT };
        std::shared_ptr<VertexBuffer> vertex_buffer;
        std::shared_ptr<IndexBuffer> index_buffer;
        std::shared_ptr<Material> material;
    };
}
