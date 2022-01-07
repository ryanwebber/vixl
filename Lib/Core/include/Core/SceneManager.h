#pragma once

#include <memory>

#include <Common/Noncopyable.h>

#include <Core/RenderTarget.h>
#include <Core/Scene.h>
#include <Core/SceneRenderer.h>

namespace Core {
    class SceneManager final {
        VX_MAKE_NONCOPYABLE(SceneManager);
        VX_MAKE_NONMOVABLE(SceneManager);
    private:
        std::unique_ptr<Scene> m_CurrentScene;

    public:
        SceneManager();

        void Update();
        void Render(RenderTarget &target);

        [[nodiscard]] const Scene& GetCurrentScene() const;
        Scene& GetCurrentScene();
        void SetCurrentScene(std::unique_ptr<Scene>);
    };
}
