#include <VX/Core/SceneManager.h>

namespace VX::Core {
    void SceneManager::set_current_scene(std::shared_ptr<Scene> current_scene) {
        if (current_scene != m_current_scene) {
            m_current_scene = std::move(current_scene);
            m_current_scene->configure();
        }
    }

    void SceneManager::update() const
    {
        if (m_current_scene != nullptr)
            m_current_scene->update();
    }
}
