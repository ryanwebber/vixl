#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Synchronization.h>
#include <VX/Graphics/Private/QueueSupport.h>
#include <VX/Graphics/Private/RenderPassImpl.h>
#include <VX/Graphics/Private/Vulkan.h>

#include <utility>

namespace VX::Graphics::Private {

    class RenderPipelineImpl final {
        VX_MAKE_NONCOPYABLE(RenderPipelineImpl);
        VX_DEFAULT_MOVABLE(RenderPipelineImpl);
    private:
        QueueSupport m_queue_support;
        std::shared_ptr<vk::raii::Device> m_device;
        std::shared_ptr<vk::raii::RenderPass> m_render_pass;

    public:
        explicit RenderPipelineImpl(QueueSupport queue_support,
                                    std::shared_ptr<vk::raii::RenderPass> render_pass,
                                    std::shared_ptr<vk::raii::Device> device)
            : m_queue_support(std::move(queue_support))
            , m_render_pass(std::move(render_pass))
            , m_device(std::move(device))
        {}

        std::optional<RenderPass> try_begin_render_pass(RenderRequest);
        void end_render_pass(RenderPass&);
    };
}
