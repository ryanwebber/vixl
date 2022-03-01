#include <VX/Graphics/RenderPipeline.h>
#include <VX/Graphics/Private/RenderPipelineImpl.h>
#include <VX/Graphics/Private/Wrappers.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics {

    namespace Private {
        void RenderPipelineImpl::submit_render_pass(Token<RenderPass> &token)
        {
            if (m_current_render_pass_holder == nullptr) {
                return;
            }

            if (m_current_render_pass_holder->reclaim(token)) {
                m_current_render_pass_holder->peek()->submit();
                m_current_render_pass_holder.reset();
            }
        }

        Token<RenderPass> RenderPipelineImpl::try_begin_render_pass(const RenderTarget &render_target,
                                                                    const RenderTiming &render_timing,
                                                                    const std::shared_ptr<CommandBuffer>& command_buffer)
        {
            if (m_current_render_pass_holder != nullptr) {
                return { };
            }

            command_buffer->impl()->reset();

            CommandRecorder command_recorder(command_buffer);
            auto render_pass = std::make_unique<RenderPass>(render_target,
                                                            render_timing,
                                                            command_recorder,
                                                            m_queue_support,
                                                            m_device,
                                                            m_render_pass);

            m_current_render_pass_holder = std::make_shared<TokenLender<RenderPass>>(std::move(render_pass));
            return m_current_render_pass_holder->borrow_token();
        }
    }

    Token<RenderPass> RenderPipeline::try_begin_render_pass(const RenderTarget &render_target,
                                                            const RenderTiming &render_timing,
                                                            const std::shared_ptr<CommandBuffer> &command_buffer)
    {
        return impl().try_begin_render_pass(render_target, render_timing, command_buffer);
    }

    void RenderPipeline::submit_render_pass(Token<RenderPass> &render_pass) {
        impl().submit_render_pass(render_pass);
    }
}
