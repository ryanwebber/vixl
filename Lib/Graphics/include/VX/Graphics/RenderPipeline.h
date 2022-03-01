#pragma once

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Token.h>

namespace VX::Graphics {

    namespace Private {
        class RenderPipelineImpl;
    }

    class RenderPipeline final: public APIObject<Private::RenderPipelineImpl> {
    public:
        using APIObject<Private::RenderPipelineImpl>::APIObject;

        Token<RenderPass> try_begin_render_pass(const RenderTarget &render_target,
                                                const RenderTiming &render_timing,
                                                const std::shared_ptr<CommandBuffer>& command_buffer);

        void submit_render_pass(Token<RenderPass>&);
    };
}
