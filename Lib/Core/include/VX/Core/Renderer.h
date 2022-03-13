#pragma once

#include <memory>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>

#include <VX/Core/RenderPass.h>
#include <VX/Core/Scene.h>

namespace VX::Core {

    class RenderTask;
    using RenderDependency = std::pair<std::reference_wrapper<const RenderTask>, std::reference_wrapper<const RenderTask>>;

    class RenderTask final {
    public:
        [[nodiscard]] RenderDependency depends_on(const RenderTask& other) const {
            return std::make_tuple(std::cref(*this), std::cref(other));
        }

        [[nodiscard]] RenderDependency dependency_of(const RenderTask& other) const {
            return std::make_tuple(std::cref(other), std::cref(*this));
        }
    };

    class RenderStage {
    public:
        virtual ~RenderStage() = default;
        virtual void render(RenderPass&) const = 0;
    };

    class RenderPipeline final {
        VX_DEFAULT_COPYABLE(RenderPipeline);
        VX_DEFAULT_MOVABLE(RenderPipeline);
    private:
        std::vector<std::shared_ptr<RenderStage>> m_render_stages { };
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        [[nodiscard]] const std::vector<std::shared_ptr<RenderStage>> &render_stages() const { return m_render_stages; }

        static RenderPipeline empty() { return {}; }
        static RenderPipeline from_scene(const Scene &) { return {}; }
    };

//    class RenderPipelineBuilder final {
//    public:
//        explicit RenderPipelineBuilder();
//        ~RenderPipelineBuilder();
//
//        RenderTask add_render_task(RenderStage);
//        RenderTask add_sub_pipeline(RenderPipeline);
//
//        void add_dependency(const RenderDependency&);
//
//        RenderPipeline Build();
//    };

    class RenderDelegate {
    public:
        virtual ~RenderDelegate() = default;

        [[nodiscard]] virtual bool is_invalidated() const = 0;
        [[nodiscard]] virtual RenderPipeline construct_render_pipeline() const = 0;
    };

    class Renderer final {
        VX_MAKE_NONCOPYABLE(Renderer);
        VX_MAKE_NONMOVABLE(Renderer);
    private:
        Graphics::Instance m_graphics;
        const RenderDelegate &m_render_delegate;

    public:
        explicit Renderer(Graphics::Instance graphics, const RenderDelegate &render_delegate)
            : m_graphics(std::move(graphics))
            , m_render_delegate(render_delegate)
        {}

        ~Renderer() = default;

        void render_frame() const;
    };
}
