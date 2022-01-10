#pragma once

#include <memory>

#include <Common/Noncopyable.h>

#include <Core/RenderContext.h>
#include <Core/Scene.h>
#include <Core/SceneRenderer.h>

namespace Core {
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
        }
    };
}
