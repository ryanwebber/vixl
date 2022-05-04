#include <VX/Copyable.h>
#include <VX/Noncopyable.h>
#include <VX/Core/SceneContext.h>
#include <VX/Core/SceneRenderer.h>

namespace VX::Core {

    class SceneRenderStage : public RenderStage {
        VX_DEFAULT_MOVABLE(SceneRenderStage);
        VX_MAKE_NONCOPYABLE(SceneRenderStage);
    private:
        std::shared_ptr<Scene> m_scene;
    public:
        explicit SceneRenderStage( std::shared_ptr<Scene> scene)
            : m_scene(std::move(scene))
        {}

        ~SceneRenderStage() override = default;

        void do_render(const RenderContext &render_context, RenderPass &render_pass) override {
            SceneContext scene_context;
            m_scene->render(scene_context, render_context, render_pass);
        }
    };

    void SceneRenderer::layout_into(RenderPipelineBuilder &builder) {
        m_stages.clear();
        m_needs_layout = false;

        if (m_active_scene != nullptr) {
            auto stage = std::make_shared<SceneRenderStage>(m_active_scene);
            builder.add_render_stage(m_render_target, stage, m_render_priority);
            m_stages.push_back(std::move(stage));
        }
    }
}
