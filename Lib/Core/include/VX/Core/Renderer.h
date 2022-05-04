#pragma once

#include <optional>
#include <span>
#include <unordered_map>

#include <VX/Graphics/Graphics.h>
#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderPass.h>
#include <VX/Core/RenderTarget.h>
#include <VX/Core/Shader.h>
#include <VX/Core/ShaderProgram.h>

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
    public:
        class Mappings final {
            VX_DEFAULT_MOVABLE(Mappings);
            VX_MAKE_NONCOPYABLE(Mappings);
        private:
            std::unordered_map<Graphics::Identifier, std::weak_ptr<RenderStage>> m_mapping;
        public:
            Mappings() = default;
            explicit Mappings(std::unordered_map<Graphics::Identifier, std::weak_ptr<RenderStage>> stage_mappings)
                : m_mapping(std::move(stage_mappings))
            {};

            [[nodiscard]] std::weak_ptr<RenderStage> lookup_stage(Graphics::Identifier identifier) const {
                auto itr = m_mapping.find(identifier);
                if (itr == m_mapping.end())
                    return { };

                return itr->second;
            }

            [[nodiscard]] std::optional<RenderTarget> lookup_target(Graphics::RenderTargetHandle handle) const {
                // TODO: Actually plumb the render target mappings, since RenderTargets
                //       have shared_ptr semantics with the underlying handle, we can't
                //       just construct one, we need an actual reference to the shared
                //       ptr from the source.
                return RenderTarget(nullptr);
            }

            ~Mappings() = default;
        };
    private:
        Graphics::SharedHandle<Graphics::HandleType::GraphicsPipeline> m_graphics_pipeline;
        Mappings m_mappings;

    public:
        explicit RenderPipeline(Graphics::SharedHandle<Graphics::HandleType::GraphicsPipeline> graphics_pipeline,
                                Mappings mappings)
            : m_graphics_pipeline(std::move(graphics_pipeline))
            , m_mappings(std::move(mappings))
        {};

        VX::Expected<void> execute(Graphics::Instance&) const;

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
                : m_render_target(std::move(render_target))
                , m_render_stage(std::move(render_stage))
                , m_render_priority(render_priority)
            {};

            [[nodiscard]] const RenderTarget &render_target() const { return m_render_target; }
            [[nodiscard]] std::weak_ptr<RenderStage> render_stage() const { return m_render_stage; }
            [[nodiscard]] RenderPriority render_priority() const { return m_render_priority; }

            ~Entry() = default;
        };

        explicit RenderPipelineBuilder() = default;
        ~RenderPipelineBuilder() = default;

        void add_render_stage(const RenderTarget&, std::weak_ptr<RenderStage>, RenderPriority);
        void add_render_dependency(RenderDependency);

        [[nodiscard]] VX::Expected<RenderPipeline> build(std::shared_ptr<Graphics::Instance>) const;

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

        VX::Expected<RenderTarget> create_render_target(const RenderTarget::BackingStore&);
        VX::Expected<Shader> create_shader(const Graphics::ShaderDescriptor&);
        VX::Expected<ShaderProgram> create_shader_program(const Graphics::GraphicsProgramDescriptor&);
    };

    class Renderer final {
        VX_MAKE_NONMOVABLE(Renderer);
        VX_MAKE_NONCOPYABLE(Renderer);
    private:
        struct State {
            bool had_construct_failure_last_frame { false };
            std::optional<RenderPipeline> render_pipeline { };
        };

        std::shared_ptr<Graphics::Instance> m_instance;

        std::shared_ptr<RenderStage> m_test { }; // TODO: Delete me

        State m_state { };
        std::vector<std::shared_ptr<RenderHook>> m_hooks { };

        void invalidate() { m_state.render_pipeline = { }; }
        VX::Expected<RenderPipeline> reconstruct();

    public:
        explicit Renderer(std::shared_ptr<Graphics::Instance> instance)
            : m_instance(std::move(instance))
        {};

        ~Renderer() = default;

        void add_hook(std::shared_ptr<RenderHook>);
        void render_frame();
    };
}
