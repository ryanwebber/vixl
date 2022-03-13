#pragma once

#include <VX/Core/Renderer.h>
#include <VX/Core/Scene.h>
#include <VX/Core/SceneRenderDelegate.h>

namespace VX::Core {
    class SceneManager final {
    private:
        SceneRenderDelegate m_render_delegate { };
        std::shared_ptr<Scene> m_current_scene { };
    public:
        explicit SceneManager() = default;
        ~SceneManager() = default;

        [[nodiscard]] const std::shared_ptr<Scene> &current_scene() const { return m_current_scene; }
        void set_current_scene(std::shared_ptr<Scene> current_scene);

        void update() const;

        [[nodiscard]] const RenderDelegate &render_delegate() const { return m_render_delegate; }
    };
}
