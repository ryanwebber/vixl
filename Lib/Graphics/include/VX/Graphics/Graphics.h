#pragma once

#include <any>
#include <bitset>
#include <memory>
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
        RenderTarget,
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
    using RenderTargetHandle = Handle<HandleType::RenderTarget>;

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

        template <HandleType H>
        void destroy(const Handle<H>&);

        VX::Expected<RenderTargetHandle> create_render_target(glm::vec<2, int> size);

        VX::Expected<GraphicsPipelineHandle> create_graphics_pipeline(const GraphicsPipelineBuilder&);

        VX::Expected<void> execute_graphics_pipeline(const GraphicsPipelineHandle&, const GraphicsDelegate&);
    };

    template<> void Instance::destroy(const Handle<HandleType::GraphicsPipeline> &);
    template<> void Instance::destroy(const Handle<HandleType::RenderTarget> &);

    struct GraphicsInfo {
        std::vector<const char*> required_extensions { };
        glm::vec<2, int> framebuffer_dimensions { };
    };

    struct GraphicsStageInfo {
        std::any user_data;
    };

    using GraphicsStageDependency = std::pair<std::reference_wrapper<const GraphicsStage>, std::reference_wrapper<const GraphicsStage>>;

    struct GraphicsStage {
        Identifier identifier;
        GraphicsStageInfo info;

        [[nodiscard]] GraphicsStageDependency depends_on(const GraphicsStage& other) const {
            return std::make_pair(std::cref(*this), std::cref(other));
        }

        [[nodiscard]] GraphicsStageDependency dependency_of(const GraphicsStage& other) const {
            return std::make_pair(std::cref(other), std::cref(*this));
        }
    };

    class GraphicsPipelineBuilder final {
        Identifier m_current_identifier { 0 };
        std::vector<GraphicsStage> m_stages { };
        std::vector<GraphicsStageDependency> m_dependencies { };
    public:
        GraphicsPipelineBuilder() = default;
        ~GraphicsPipelineBuilder() = default;

        GraphicsStage add_stage(const GraphicsStageInfo &stage_info) {
            GraphicsStage stage = {
                    .identifier = ++m_current_identifier,
                    .info = stage_info,
            };

            m_stages.push_back(stage);
            return stage;
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
}
