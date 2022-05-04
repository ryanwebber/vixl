#pragma once

#include <cstdint>

#include <VX/Expected.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Core/Material.h>

namespace VX::Core {
    class RenderPass final {
        VX_MAKE_NONMOVABLE(RenderPass);
        VX_MAKE_NONCOPYABLE(RenderPass);
    private:
        Graphics::GraphicsContext &m_graphics_context;
    public:
        explicit RenderPass(Graphics::GraphicsContext &graphics_context)
            : m_graphics_context(graphics_context)
        {};

        ~RenderPass() = default;

        void bind_material(const Material&);
        void draw(uint32_t vertex_count);
    };
}
