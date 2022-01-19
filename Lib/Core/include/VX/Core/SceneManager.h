#pragma once

#include <memory>

#include <VX/Noncopyable.h>

#include <VX/Core/RenderContext.h>
#include <VX/Core/Scene.h>
#include <VX/Core/SceneRenderer.h>

namespace VX::Core {
    class SceneManager final {
        VX_MAKE_NONCOPYABLE(SceneManager);
        VX_MAKE_NONMOVABLE(SceneManager);
    private:
        std::shared_ptr<Scene> m_current_scene;

    public:
        SceneManager();

        void update();
        void render(RenderContext&);

        void set_active_scene(std::shared_ptr<Scene> scene) {
            m_current_scene = std::move(scene);
            m_current_scene->configure();
        }
    };
}
