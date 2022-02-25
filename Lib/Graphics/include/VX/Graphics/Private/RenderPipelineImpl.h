#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Token.h>
#include <VX/Graphics/Private/RenderPassImpl.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {

    class RenderPipelineImpl final {
        VX_MAKE_NONCOPYABLE(RenderPipelineImpl);
        VX_DEFAULT_MOVABLE(RenderPipelineImpl);
    private:
        std::shared_ptr<vk::raii::RenderPass> m_render_pass;
        std::shared_ptr<TokenLender<RenderPass>> m_current_render_pass_holder;

    public:
        explicit RenderPipelineImpl(std::shared_ptr<vk::raii::RenderPass> render_pass)
            : m_render_pass(std::move(render_pass))
            , m_current_render_pass_holder(nullptr)
        {}

        Token<RenderPass> try_begin_render_pass(const RenderTarget &render_target) {
            if (m_current_render_pass_holder != nullptr) {
                return { };
            }

            auto render_pass = std::make_unique<RenderPass>(m_render_pass, render_target.allocated_command_buffer());
            m_current_render_pass_holder = std::make_shared<TokenLender<RenderPass>>(std::move(render_pass));
            return m_current_render_pass_holder->borrow_token();
        }

        void end_render_pass(Token<RenderPass> &token) {
            if (m_current_render_pass_holder == nullptr) {
                return;
            }

            if (m_current_render_pass_holder->reclaim(token)) {
                m_current_render_pass_holder.reset();
            }
        }
    };
}
