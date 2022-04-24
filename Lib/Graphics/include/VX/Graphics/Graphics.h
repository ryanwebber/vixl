#pragma once

#include <any>
#include <bitset>
#include <cstddef>
#include <memory>
#include <span>
#include <vector>

#include <glm/glm.hpp>

#include <VX/Copyable.h>
#include <VX/Expected.h>
#include <VX/Noncopyable.h>

namespace VX::Graphics {

    class GraphicsDelegate;
    class GraphicsPipelineBuilder;
    class GraphicsStage;
    class Instance;
    class InstanceImpl;

    using Flags = std::bitset<32>;
    using Identifier = int32_t;

    enum class HandleType {
        GraphicsPipeline,
        GraphicsProgram,
        RenderTarget,
        Shader,
    };

    template <HandleType H>
    class Handle final {
        VX_DEFAULT_MOVABLE(Handle);
        VX_DEFAULT_COPYABLE(Handle);
    private:
        HandleType m_type;
        Identifier m_identifier;
        Flags m_flags;
    public:
        explicit Handle(Identifier identifier, Flags flags = { })
            : m_type(H)
            , m_identifier(identifier)
            , m_flags(flags)
        {};

        [[nodiscard]] HandleType type() const { return m_type; }
        [[nodiscard]] Identifier identifier() const { return m_identifier; }
        [[nodiscard]] Flags flags() const { return m_flags; }
    };

    using GraphicsPipelineHandle = Handle<HandleType::GraphicsPipeline>;
    using GraphicsProgramHandle = Handle<HandleType::GraphicsProgram>;
    using RenderTargetHandle = Handle<HandleType::RenderTarget>;
    using ShaderHandle = Handle<HandleType::Shader>;

    struct ShaderDescriptor {
        enum class Type { Fragment, Vertex };
        Type type;
    };

    struct GraphicsProgramDescriptor {
        ShaderHandle vertex_shader;
        ShaderHandle fragment_shader;
    };

    class SwapchainInfo final {
    private:
        const Instance& m_instance;
    public:
        explicit SwapchainInfo(const Instance& instance)
            : m_instance(instance)
        {};

        ~SwapchainInfo() = default;

        [[nodiscard]] RenderTargetHandle render_target() const;
    };

    class Instance final {
    private:
        std::shared_ptr<InstanceImpl> m_impl;
    public:
        explicit Instance(std::shared_ptr<InstanceImpl>);

        Instance(Instance&&) noexcept;
        Instance& operator=(Instance&&) noexcept;

        Instance(const Instance&) noexcept;
        Instance& operator=(const Instance&) noexcept;

        ~Instance();

        const InstanceImpl* operator->() const;
        InstanceImpl* operator->();

        const InstanceImpl &operator*() const;
        InstanceImpl &operator*();

        // API

        void destroy(const Handle<HandleType::GraphicsPipeline> &);
        void destroy(const Handle<HandleType::GraphicsProgram> &);
        void destroy(const Handle<HandleType::RenderTarget> &);
        void destroy(const Handle<HandleType::Shader> &);

        [[nodiscard]] SwapchainInfo swapchain_info() const;
        [[nodiscard]] VX::Expected<RenderTargetHandle> create_render_target(glm::vec<2, int> size);
        [[nodiscard]] VX::Expected<ShaderHandle> create_shader(const ShaderDescriptor&, std::span<const std::byte> program_data);
        [[nodiscard]] VX::Expected<GraphicsProgramHandle> create_program(const GraphicsProgramDescriptor&);
        [[nodiscard]] VX::Expected<GraphicsPipelineHandle> create_graphics_pipeline(const GraphicsPipelineBuilder&);
        VX::Expected<void> execute_graphics_pipeline(const GraphicsPipelineHandle&, const GraphicsDelegate&);
    };

    struct GraphicsInfo {
        std::vector<const char*> required_extensions { };
        glm::vec<2, int> framebuffer_dimensions { };
    };

    struct GraphicsStageInfo {
        std::any user_data;
        RenderTargetHandle render_target;
    };

    struct GraphicsStageDependency {
        const Identifier &dependee;
        const Identifier &dependent;

        struct Partial {
            const Identifier &dependee;

            [[nodiscard]] GraphicsStageDependency to(const Identifier& other) const {
                return {
                    .dependee = dependee,
                    .dependent = other,
                };
            }
        };

        static Partial from(const Identifier& target) {
            return {
                .dependee = target,
            };
        }
    };

    struct GraphicsStage {
        Identifier identifier;
        GraphicsStageInfo info;
    };

    class GraphicsPipelineBuilder final {
        Identifier m_current_identifier { 0 };
        std::vector<GraphicsStage> m_stages { };
        std::vector<GraphicsStageDependency> m_dependencies { };
    public:
        GraphicsPipelineBuilder() = default;
        ~GraphicsPipelineBuilder() = default;

        Identifier add_stage(const GraphicsStageInfo &stage_info) {
            Identifier  identifier = ++m_current_identifier;
            GraphicsStage stage = {
                    .identifier = identifier,
                    .info = stage_info,
            };

            m_stages.push_back(stage);
            return identifier;
        }

        void add_dependency(const GraphicsStageDependency &dep) {
            m_dependencies.push_back(dep);
        }
    };

    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
    };

    class GraphicsDelegate {
    public:
        virtual ~GraphicsDelegate() = default;
        virtual void handle_stage(const GraphicsStage& stage, GraphicsContext&) const = 0;
    };

    template <HandleType H>
    using SharedHandle = std::shared_ptr<Handle<H>>;

    template <HandleType H>
    SharedHandle<H> to_shared_handle(std::weak_ptr<Instance> instance, Handle<H> handle) {
        return std::shared_ptr<Handle<H>>(new Handle<H>(handle), [weak_instance = std::move(instance)](Handle<H> *inner_handle){
            if (auto strong_instance = weak_instance.lock()) {
                strong_instance->destroy(*inner_handle);
                delete inner_handle;
            }
        });
    }
}
