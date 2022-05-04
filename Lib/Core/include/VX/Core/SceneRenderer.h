#pragma once

#include <memory>
#include <unordered_map>

#include <VX/Core/Renderer.h>
#include <VX/Core/Scene.h>
#include <VX/Core/SceneManager.h>

namespace VX::Core {
    class SceneRenderer : RenderHook {
    private:
        bool m_needs_layout { false };
        std::shared_ptr<Scene> m_active_scene { nullptr };
        std::vector<std::shared_ptr<RenderStage>> m_stages { };

        RenderTarget m_render_target;
        RenderPriority m_render_priority;

    public:
        explicit SceneRenderer(RenderTarget render_target, RenderPriority render_priority)
            : m_render_target(std::move(render_target))
            , m_render_priority(render_priority)
        {};

        ~SceneRenderer() override = default;

        [[nodiscard]] const std::shared_ptr<Scene> active_scene() const  { return m_active_scene; }
        void set_active_scene(std::shared_ptr<Scene> scene) {
            m_active_scene = std::move(scene);
            m_needs_layout = true;
        }

        bool needs_layout() override {
            return m_needs_layout;
        }

        void layout_into(RenderPipelineBuilder&) override;
    };

    class CinematicSceneRenderer final : public RenderHook {
    private:
        SceneRenderer m_scene_renderer;
        std::shared_ptr<SceneManager> m_scene_manager;
    public:
        template <class ...Args>
        explicit CinematicSceneRenderer(std::shared_ptr<SceneManager> scene_manager, Args&&...args)
            : m_scene_renderer(std::forward<Args>(args)...)
            , m_scene_manager(std::move(scene_manager))
        {
            // TODO: register a callback for scene changes
            m_scene_renderer.set_active_scene(m_scene_manager->current_scene());
        }

        ~CinematicSceneRenderer() override = default;

        bool needs_layout() override {
            return m_scene_renderer.needs_layout();
        }

        void layout_into(RenderPipelineBuilder &builder) override {
            m_scene_renderer.layout_into(builder);
        }
    };
}
