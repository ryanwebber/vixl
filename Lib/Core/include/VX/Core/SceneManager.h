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
        std::shared_ptr<Scene> m_CurrentScene;

    public:
        SceneManager();

        void Update();
        void Render(RenderContext&);

        void SetActiveScene(std::shared_ptr<Scene> scene) {
            m_CurrentScene = std::move(scene);
            m_CurrentScene->Configure();
        }
    };
}
