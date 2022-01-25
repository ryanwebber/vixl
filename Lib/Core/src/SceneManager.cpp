#include <VX/Core/SceneManager.h>

namespace VX::Core {
    SceneManager::SceneManager()
        : m_current_scene(nullptr)
    {
    }

    void SceneManager::update() {
        if (m_current_scene == nullptr)
            return;

        m_current_scene->update();
    }

    void SceneManager::render(RenderContext &context, const RenderTarget &target) {
        if (m_current_scene == nullptr)
            return;

        m_current_scene->render(context, target);
    }
}
