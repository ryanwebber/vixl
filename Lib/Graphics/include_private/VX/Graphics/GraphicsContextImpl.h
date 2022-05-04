#pragma once

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Memory.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {
    class GraphicsContextImpl: public GraphicsContext {
        VX_MAKE_NONMOVABLE(GraphicsContextImpl);
        VX_MAKE_NONCOPYABLE(GraphicsContextImpl);
    private:
        const InstanceImpl &m_instance;
        const RenderPass &m_render_pass;

        [[nodiscard]] const ResourceManager::HandleAllocatorCollection &handles() const {
            return m_instance.resource_manager().handle_allocators();
        }

        [[nodiscard]] const vk::raii::CommandBuffer &command_buffer() const {
            return *(m_render_pass.render_context().command_buffer);
        }

    public:
        GraphicsContextImpl(const InstanceImpl &instance, const RenderPass &render_pass)
                : m_instance(instance)
                , m_render_pass(render_pass)
        {};

        void bind_program(const GraphicsProgramHandle&) override;
        void draw(uint32_t vertex_count) override;

        ~GraphicsContextImpl() override = default;
    };
}
