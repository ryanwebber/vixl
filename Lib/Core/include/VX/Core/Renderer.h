#pragma once

#include <span>

#include <VX/Graphics/Graphics.h>
#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderPass.h>
#include <VX/Core/RenderTarget.h>

namespace VX::Core {

    class RenderHook;
    class RenderPipeline;
    class RenderPipelineBuilder;
    class RenderStage;
    class Renderer;

    using RenderPriority = int;

    struct RenderDependency {
        const RenderTarget &dependee;
        const RenderTarget &dependent;

        struct Partial {
            const RenderTarget &dependee;

            [[nodiscard]] RenderDependency to(const RenderTarget& other) const {
                return {
                    .dependee = dependee,
                    .dependent = other,
                };
            }
        };

        static Partial from(const RenderTarget& target) {
            return {
                .dependee = target,
            };
        }
    };

    class RenderStage {
    public:
        virtual ~RenderStage() = default;
        virtual void do_render(const RenderContext&, RenderPass&) = 0;
    };

    class RenderHook {
    public:
        virtual ~RenderHook() = default;

        virtual bool needs_layout() = 0;
        virtual void layout_into(RenderPipelineBuilder&) = 0;
    };

    class RenderPipeline final {
        VX_DEFAULT_MOVABLE(RenderPipeline);
        VX_MAKE_NONCOPYABLE(RenderPipeline);
    private:
        std::shared_ptr<Graphics::GraphicsPipelineHandle> m_graphics_pipeline;
    public:
        explicit RenderPipeline(std::shared_ptr<Graphics::GraphicsPipelineHandle> graphics_pipeline)
            : m_graphics_pipeline(std::move(graphics_pipeline))
        {};

        ~RenderPipeline() = default;
    };

    class RenderPipelineBuilder final {
        VX_MAKE_NONMOVABLE(RenderPipelineBuilder);
        VX_MAKE_NONCOPYABLE(RenderPipelineBuilder);
    public:
        class Entry {
            VX_DEFAULT_MOVABLE(Entry);
            VX_DEFAULT_COPYABLE(Entry);
        private:
            RenderTarget m_render_target;
            std::weak_ptr<RenderStage> m_render_stage;
            RenderPriority m_render_priority;
        public:
            Entry(RenderTarget render_target, std::weak_ptr<RenderStage> render_stage, RenderPriority render_priority)
                : m_render_target(render_target)
                , m_render_stage(render_stage)
                , m_render_priority(render_priority)
            {};

            ~Entry() = default;
        };

        explicit RenderPipelineBuilder() = default;
        ~RenderPipelineBuilder() = default;

        void add_render_stage(const RenderTarget&, std::weak_ptr<RenderStage>, RenderPriority);
        void add_render_dependency(RenderDependency);

        [[nodiscard]] RenderPipeline build() const;

    private:
        std::vector<Entry> m_entries { };
        std::vector<RenderDependency> m_dependencies { };
    };

    class RenderAllocator final {
        VX_DEFAULT_MOVABLE(RenderAllocator);
        VX_DEFAULT_COPYABLE(RenderAllocator);
    private:
        std::shared_ptr<Graphics::Instance> m_instance;
    public:
        explicit RenderAllocator(std::shared_ptr<Graphics::Instance> instance)
            : m_instance(std::move(instance))
        {};

        ~RenderAllocator() = default;

        RenderTarget create_render_target(const RenderTarget::AllocationRequest&);
    };

    class Renderer final {
        VX_DEFAULT_MOVABLE(Renderer);
        VX_MAKE_NONCOPYABLE(Renderer);
    private:
        std::shared_ptr<Graphics::Instance> m_instance;
        std::vector<std::shared_ptr<RenderHook>> m_hooks { };
    public:
        explicit Renderer(std::shared_ptr<Graphics::Instance> instance)
            : m_instance(std::move(instance))
        {};

        ~Renderer() = default;

        void add_hook();
        void render_frame() const;
    };
}
