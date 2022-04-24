#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Memory.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    class Shader final {
        VX_DEFAULT_MOVABLE(Shader);
        VX_MAKE_NONCOPYABLE(Shader);
    private:
        ShaderDescriptor m_descriptor;
        vk::raii::ShaderModule m_shader_module;
    public:
        Shader(ShaderDescriptor descriptor, vk::raii::ShaderModule shader_module)
            : m_descriptor(descriptor)
            , m_shader_module(std::move(shader_module))
        {}

        [[nodiscard]] const vk::raii::ShaderModule &module() const { return m_shader_module; }

        ~Shader() = default;
    };

    class GraphicsProgram final {
        VX_DEFAULT_MOVABLE(GraphicsProgram);
        VX_MAKE_NONCOPYABLE(GraphicsProgram);
    private:
        vk::raii::PipelineLayout m_pipeline_layout;
        vk::raii::Pipeline m_pipeline;

    public:
        GraphicsProgram(vk::raii::PipelineLayout pipeline_layout, vk::raii::Pipeline pipeline)
            : m_pipeline_layout(std::move(pipeline_layout))
            , m_pipeline(std::move(pipeline))
        {
        }

        ~GraphicsProgram() = default;
    };
}
