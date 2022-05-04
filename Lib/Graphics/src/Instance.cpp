#include <VX/ArrayUtils.h>
#include <VX/Try.h>
#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/GraphicsPipeline.h>
#include <VX/Graphics/GraphicsProgram.h>
#include <VX/Graphics/Instance.h>

namespace VX::Graphics {

    Instance::Instance(std::unique_ptr<InstanceImpl> impl)
        : m_impl(std::move(impl))
    {}

    Instance::Instance(Instance&&) noexcept = default;
    Instance& Instance::operator=(Instance&&) noexcept = default;

    Instance::~Instance() = default;

    const InstanceImpl* Instance::operator->() const { return m_impl.get(); }
    InstanceImpl* Instance::operator->() { return m_impl.get(); }

    const InstanceImpl &Instance::operator*() const { return *m_impl; }
    InstanceImpl &Instance::operator*() { return *m_impl; }

    void Instance::destroy(const Handle<HandleType::GraphicsPipeline> &handle) {
        m_impl->resource_manager().handle_allocators().graphics_pipeline.destroy(handle);
    }

    void Instance::destroy(const Handle<HandleType::GraphicsProgram> &handle) {
        m_impl->resource_manager().handle_allocators().graphics_program.destroy(handle);
    }

    void Instance::destroy(const Handle<HandleType::RenderTarget> &handle) {
        m_impl->resource_manager().handle_allocators().render_target.destroy(handle);
    }

    void Instance::destroy(const Handle<HandleType::Shader> &handle) {
        m_impl->resource_manager().handle_allocators().shader.destroy(handle);
    }

    SwapchainInfo Instance::swapchain_info() const {
        return SwapchainInfo(*this);
    }

    VX::Expected<RenderTargetHandle> Instance::create_render_target(glm::vec<2, int> size)
    {
        return VX::make_unexpected("Not implemented");
    }

    VX::Expected<ShaderHandle> Instance::create_shader(const ShaderDescriptor &descriptor,
                                                       std::span<const std::byte> program_data) {
        return m_impl->create_shader(descriptor, program_data);
    }

    [[nodiscard]] VX::Expected<GraphicsProgramHandle> Instance::create_program(const GraphicsProgramDescriptor &descriptor) {
        return m_impl->create_program(descriptor);
    }

    VX::Expected<GraphicsPipelineHandle> Instance::create_graphics_pipeline(const GraphicsPipelineBuilder &)
    {
        // TODO: accurately create a pipeline
        GraphicsStage stage = {
                .identifier = 1,
                .info = {
                        .user_data = { },
                        .render_target = RenderTargetHandle(0)
                }
        };

        std::vector<std::unique_ptr<GraphicsStageExecutor>> executors;
        executors.push_back(std::make_unique<RenderFrameStage>(stage));

        auto& allocator = m_impl->resource_manager().handle_allocators().graphics_pipeline;
        return allocator.allocate<HandleType::GraphicsPipeline>(std::move(executors));
    }

    VX::Expected<void> Instance::execute_graphics_pipeline(const GraphicsPipelineHandle &handle, const GraphicsDelegate &delegate)
    {
        auto& allocator = m_impl->resource_manager().handle_allocators().graphics_pipeline;
        return allocator.lookup(handle).execute(*m_impl, delegate);
    }

    void Instance::wait_for_idle()
    {
        m_impl->wait_for_idle();
    }

    // Instance Impl

    VX::Expected<RenderPass> InstanceImpl::begin_render_pass(const RenderTarget &target, const RenderContext &context)
    {
        vk::CommandBufferBeginInfo command_buffer_begin_info = {};

        vk::ClearValue clear_values[] = {
                {
                        .color = {{{ 0.008f, 0.008f, 0.008f, 0.000f }}},
                }
        };

        vk::RenderPassBeginInfo render_pass_begin_info = {
                .renderPass = &(**m_render_pass),
                .framebuffer = &(**target.framebuffer()),
                .renderArea = {
                        .offset = {
                                .x = 0,
                                .y = 0,
                        },
                        .extent = target.extents()
                },
                .clearValueCount = 1,
                .pClearValues = clear_values
        };

        auto& command_buffer = *context.command_buffer;
        command_buffer.reset();
        command_buffer.begin(command_buffer_begin_info);
        command_buffer.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

        return RenderPass(context);
    }

    VX::Expected<void> InstanceImpl::end_render_pass(const RenderPass &render_pass)
    {
        vk::PipelineStageFlags wait_stage_flags[] = {
                vk::PipelineStageFlagBits::eColorAttachmentOutput
        };

        const auto k_maximum_semaphores = VX::capacity<SemaphoreBuffer>();

        size_t n_signal_semaphores;
        vk::Semaphore signal_semaphores[k_maximum_semaphores];
        for (n_signal_semaphores = 0; n_signal_semaphores < k_maximum_semaphores; n_signal_semaphores++) {
            if (render_pass.render_context().signal_semaphores[n_signal_semaphores])
                signal_semaphores[n_signal_semaphores] = ***render_pass.render_context().signal_semaphores[n_signal_semaphores];
            else
                break;
        }

        size_t n_wait_semaphores;
        vk::Semaphore wait_semaphores[k_maximum_semaphores];
        for (n_wait_semaphores = 0; n_wait_semaphores < k_maximum_semaphores; n_wait_semaphores++) {
            if (render_pass.render_context().wait_semaphores[n_wait_semaphores])
                wait_semaphores[n_wait_semaphores] = ***render_pass.render_context().wait_semaphores[n_wait_semaphores];
            else
                break;
        }

        vk::CommandBuffer command_buffers[] = {
                **render_pass.render_context().command_buffer
        };

        vk::SubmitInfo submit_info = {
                .waitSemaphoreCount = static_cast<uint32_t>(n_wait_semaphores),
                .pWaitSemaphores = wait_semaphores,
                .pWaitDstStageMask = wait_stage_flags,
                .commandBufferCount = 1,
                .pCommandBuffers = command_buffers,
                .signalSemaphoreCount = static_cast<uint32_t>(n_signal_semaphores),
                .pSignalSemaphores = signal_semaphores,
        };

        auto queue = m_logical_device->getQueue(m_queue_support.get_queue<QueueFeature::Graphics>(), 0);

        return VX::try_catch<vk::Error>([&](){
            render_pass.render_context().command_buffer->endRenderPass();
            render_pass.render_context().command_buffer->end();
            queue.submit({submit_info}, **render_pass.render_context().signal_fence);
        });
    }

    VX::Expected<ShaderHandle> InstanceImpl::create_shader(const ShaderDescriptor &descriptor,
                                                           std::span<const std::byte> program_data)
    {
        vk::ShaderModuleCreateInfo create_info = {
                .codeSize = program_data.size(),
                .pCode = reinterpret_cast<const uint32_t*>(program_data.data()),
        };

        auto maybe_shader_module = VX::try_catch<vk::raii::ShaderModule, vk::Error>([&]() {
            return m_logical_device->createShaderModule(create_info);
        });

        if (!maybe_shader_module.has_value())
            return VX::map_unexpected(maybe_shader_module);

        auto &allocator = m_resource_manager.handle_allocators().shader;
        return allocator.allocate<HandleType::Shader>(descriptor, std::move(maybe_shader_module.value()));
    }

    VX::Expected<GraphicsProgramHandle> InstanceImpl::create_program(const GraphicsProgramDescriptor &descriptor) {

        const auto &vertex_shader = m_resource_manager.handle_allocators().shader.lookup(descriptor.vertex_shader);
        const auto &fragment_shader = m_resource_manager.handle_allocators().shader.lookup(descriptor.fragment_shader);

        vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info = { };

        auto pipeline_layout = m_logical_device->createPipelineLayout({
            // TODO: pipeline layout stuff for uniforms etc...
        });

        vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
                .topology = vk::PrimitiveTopology::eTriangleList,
                .primitiveRestartEnable = false,
        };

        // TODO: Use dynamic states for this
        vk::Viewport viewport = {
                .x = 0.0f,
                .y = 0.0f,

                .width = 800.0f,
                .height = 600.0f,

                .minDepth = 0.0f,
                .maxDepth = 1.0f,
        };

        // TODO: Use dynamic states for this
        vk::Rect2D scissor = {
                .offset = { 0, 0 },

                // Ditto the comment above about sizing
                .extent = { 800, 600 },
        };

        vk::PipelineViewportStateCreateInfo viewport_state_create_info = {
                .viewportCount = 1,
                .pViewports = &viewport,
                .scissorCount = 1,
                .pScissors = &scissor,
        };

        vk::PipelineRasterizationStateCreateInfo rasterization_state_create_info = {
                .depthClampEnable = false,
                .rasterizerDiscardEnable = false,
                .polygonMode = vk::PolygonMode::eFill,
                .cullMode = vk::CullModeFlagBits::eBack,
                .frontFace = vk::FrontFace::eClockwise,
                .depthBiasEnable = false,
                .lineWidth = 1.0f,
        };

        vk::PipelineMultisampleStateCreateInfo multisample_state_create_info = {
                .rasterizationSamples = vk::SampleCountFlagBits::e1,
                .sampleShadingEnable = false,
        };

        vk::PipelineColorBlendAttachmentState color_blend_attachment_state = {
                .blendEnable = true,
                .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
                .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
                .colorBlendOp = vk::BlendOp::eAdd,
                .srcAlphaBlendFactor = vk::BlendFactor::eOne,
                .dstAlphaBlendFactor = vk::BlendFactor::eZero,
                .alphaBlendOp = vk::BlendOp::eAdd,
                .colorWriteMask = vk::ColorComponentFlagBits::eR
                                | vk::ColorComponentFlagBits::eG
                                | vk::ColorComponentFlagBits::eB
                                | vk::ColorComponentFlagBits::eA
        };

        vk::PipelineColorBlendStateCreateInfo color_blend_state_create_info = {
                .logicOpEnable = false,
                .logicOp = vk::LogicOp::eCopy,
                .attachmentCount = 1,
                .pAttachments = &color_blend_attachment_state,
        };

        vk::PipelineShaderStageCreateInfo shader_stages[] = {
                {
                        .stage = vk::ShaderStageFlagBits::eVertex,
                        .module = *vertex_shader.module(),
                        .pName = "main",
                },
                {
                        .stage = vk::ShaderStageFlagBits::eFragment,
                        .module = *fragment_shader.module(),
                        .pName = "main",
                },
        };

        vk::GraphicsPipelineCreateInfo pipeline_create_info = {
                .stageCount = 2,
                .pStages = &shader_stages[0],
                .pVertexInputState = &vertex_input_state_create_info,
                .pInputAssemblyState = &input_assembly_state_create_info,
                .pViewportState = &viewport_state_create_info,
                .pRasterizationState = &rasterization_state_create_info,
                .pMultisampleState = &multisample_state_create_info,
                .pDepthStencilState = nullptr,
                .pColorBlendState = &color_blend_state_create_info,
                .pDynamicState = nullptr,

                .layout = *pipeline_layout,
                .renderPass = *m_render_pass,
                .subpass = 0,
        };

        auto pipeline = m_logical_device->createGraphicsPipeline({ nullptr }, pipeline_create_info);

        auto &allocator = m_resource_manager.handle_allocators().graphics_program;
        return allocator.allocate<HandleType::GraphicsProgram>(std::move(pipeline_layout), std::move(pipeline));
    }

    void InstanceImpl::wait_for_idle() {
        m_logical_device->waitIdle();
    }
}
