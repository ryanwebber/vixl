#include <VX/Core/SceneManager.h>

namespace VX::Core {
    void SceneManager::set_current_scene(std::shared_ptr<Scene> current_scene) {
        if (current_scene != m_current_scene) {
            m_current_scene = current_scene;
            m_render_delegate.set_current_scene(current_scene);
        }
    }

    void SceneManager::update() const
    {
        if (m_current_scene != nullptr)
            m_current_scene->update();
    }
}
