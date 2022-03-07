#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class GraphicsPipelineImpl final {
        VX_MAKE_NONCOPYABLE(GraphicsPipelineImpl);
        VX_DEFAULT_MOVABLE(GraphicsPipelineImpl);
    private:
        vk::raii::ShaderModule m_vertex_shader_module;
        vk::raii::ShaderModule m_fragment_shader_module;
        vk::raii::PipelineLayout m_pipeline_layout;
        vk::raii::Pipeline m_pipeline;

    public:
        GraphicsPipelineImpl(vk::raii::ShaderModule vertex_shader_module,
                             vk::raii::ShaderModule fragment_shader_module,
                             vk::raii::PipelineLayout pipeline_layout,
                             vk::raii::Pipeline pipeline)
                : m_vertex_shader_module(std::move(vertex_shader_module))
                , m_fragment_shader_module(std::move(fragment_shader_module))
                , m_pipeline_layout(std::move(pipeline_layout))
                , m_pipeline(std::move(pipeline))
        {}

        ~GraphicsPipelineImpl() = default;

        vk::raii::Pipeline &pipeline() { return m_pipeline; }
        [[nodiscard]] const vk::raii::Pipeline &pipeline() const { return m_pipeline; }
    };
}
