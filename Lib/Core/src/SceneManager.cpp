#include <Core/SceneManager.h>

namespace Core {
    SceneManager::SceneManager()
        : m_CurrentScene(nullptr)
    {
    }

    void SceneManager::Update() {
        if (m_CurrentScene != nullptr) {
            m_CurrentScene->Update();
        }
    }

    void SceneManager::Render(RenderTarget &target) {
        if (m_CurrentScene != nullptr) {
            m_CurrentScene->Render(target);
        }
    }

    Scene &SceneManager::GetCurrentScene() {
        return *m_CurrentScene;
    }

    const Scene &SceneManager::GetCurrentScene() const {
        return *m_CurrentScene;
    }

    void SceneManager::SetCurrentScene(std::unique_ptr<Scene> scene) {
        if (m_CurrentScene != nullptr)
            m_CurrentScene->Reset();

        m_CurrentScene = std::move(scene);
    }
}
