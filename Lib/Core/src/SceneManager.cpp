#include <Core/SceneManager.h>

namespace Core {
    SceneManager::SceneManager()
        : m_CurrentScene(nullptr)
    {
    }

    void SceneManager::Update() {
        if (m_CurrentScene == nullptr)
            return;

        m_CurrentScene->Update();
    }

    void SceneManager::Render(RenderTarget &target) {
        if (!target.IsEnabled() || m_CurrentScene == nullptr)
            return;

        m_CurrentScene->Render(target);
    }
}
