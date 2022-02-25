#pragma once

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/Token.h>

namespace VX::Graphics {

    namespace Private {
        class RenderPipelineImpl;
    }

    class RenderPipeline final: public APIObject<Private::RenderPipelineImpl> {
    public:
        using APIObject<Private::RenderPipelineImpl>::APIObject;

        Token<RenderPass> try_begin_render_pass(std::shared_ptr<Framebuffer> target);
        void end_render_pass(Token<RenderPass>&);
    };
}
