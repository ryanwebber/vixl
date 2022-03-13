#pragma once

#include <memory>

#include <VX/Core/Renderer.h>
#include <VX/Core/Scene.h>

namespace VX::Core {
    class SceneRenderDelegate: public RenderDelegate {
    private:
        bool m_invalidated { false };
        std::shared_ptr<Scene> m_current_scene { };
    public:
        SceneRenderDelegate() = default;
        ~SceneRenderDelegate() override = default;

        void set_current_scene(std::shared_ptr<Scene> current_scene) {
            if (current_scene != m_current_scene) {
                m_current_scene = std::move(current_scene);
                m_invalidated = true;
            }
        }

        [[nodiscard]] bool is_invalidated() const override { return m_invalidated; }
        [[nodiscard]] RenderPipeline construct_render_pipeline() const override {
            if (m_current_scene == nullptr)
                return RenderPipeline::empty();

            return RenderPipeline::from_scene(*m_current_scene);
        }
    };
}