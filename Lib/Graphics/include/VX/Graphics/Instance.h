#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Copyable.h>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/Framebuffer.h>
#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/RenderPipeline.h>
#include <VX/Graphics/Swapchain.h>

namespace VX::Graphics {

    namespace Private {
        class InstanceImpl;
    }

    class Instance final : public APIObject<Private::InstanceImpl>, private std::enable_shared_from_this<Instance> {
    public:
        using APIObject<Private::InstanceImpl>::APIObject;

        const RenderPipeline& render_pipeline() const;
        RenderPipeline& render_pipeline();

        const Swapchain& swapchain() const;
        Swapchain& swapchain();

        const GraphicsPipeline& example_graphics_pipeline() const;
        GraphicsPipeline& example_graphics_pipeline();

        // APIs for:
        //  * Graphics pipeline creation
        //  * Creating new render targets
        //  * Mipmap generation?
    };
}
