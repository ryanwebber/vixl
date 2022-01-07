#include <Core/Scene.h>

namespace Core {
    void Scene::Update() {
        for (auto &&system : m_UpdateSystems)
            system->Update();
    }

    void Scene::Render(RenderCamera &target) {
        SceneCamera camera;
        for (auto &&system : m_RenderSystems)
            system->Render(target, camera);
    }

    void Scene::Reset() {
        m_UpdateSystems.clear();
        m_RenderSystems.clear();
    }
}
