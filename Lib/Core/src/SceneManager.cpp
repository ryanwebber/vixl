#include <VX/Core/SceneManager.h>

namespace VX::Core {
    SceneManager::SceneManager()
        : m_CurrentScene(nullptr)
    {
    }

    void SceneManager::Update() {
        if (m_CurrentScene == nullptr)
            return;

        m_CurrentScene->Update();
    }

    void SceneManager::Render(RenderContext &context) {
        if (m_CurrentScene == nullptr)
            return;

        m_CurrentScene->Render(context);
    }
}
