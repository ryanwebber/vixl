#pragma once

#include <memory>
#include <span>
#include <vector>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Assert.h>
#include <VX/Graphics/RenderContext.h>
#include <VX/Graphics/Memory.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    class RenderPass final {
        VX_DEFAULT_MOVABLE(RenderPass);
        VX_DEFAULT_COPYABLE(RenderPass);
    private:
        class GraphicsContextImpl: public GraphicsContext {

        };

        RenderContext m_render_context;
        GraphicsContextImpl m_graphics_context_impl;
    public:
        explicit RenderPass(RenderContext render_context)
            : m_render_context(std::move(render_context))
            , m_graphics_context_impl()
        {}

        ~RenderPass() = default;

        [[nodiscard]] const RenderContext &render_context() const { return m_render_context; }

        GraphicsContext &graphics_context() { return m_graphics_context_impl; }
        [[nodiscard]] const GraphicsContext &graphics_context() const { return m_graphics_context_impl; }
    };
}
