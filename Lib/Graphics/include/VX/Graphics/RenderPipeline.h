#pragma once

#include <optional>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/RenderRequest.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Ticket.h>

namespace VX::Graphics {

    namespace Private {
        class RenderPipelineImpl;
    }

    class RenderPipeline final: public APIObject<Private::RenderPipelineImpl> {
    public:
        using APIObject<Private::RenderPipelineImpl>::APIObject;

        std::optional<RenderPass> try_begin_render_pass(RenderRequest);
        void end_render_pass(RenderPass&);
    };
}
