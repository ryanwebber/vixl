#include <glm/glm.hpp>
#include <VX/Core/Scene.h>

namespace VX::Core {
    void Scene::Configure() {
        auto weak_self = WeakSelf();
        for (auto &&system : m_MetaSystems)
            system->Configure(weak_self);
    }

    void Scene::Update() {
        for (auto &&system : m_UpdateSystems)
            system->Update(m_EntityRegistry);
    }

    void Scene::Render(RenderContext &context) {

        glm::vec3 eye(0.0f, 0.0f, -5.0f);
        glm::vec3 center(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, -1.0f, 0.0f); // TODO: Graphics dependent up/down not working, setting for metal for now
        auto view_matrix = glm::lookAt(eye, center, up);
        auto proj_matrix = glm::perspective(80.0f, (800.0f / 600.0f), 0.1f, 100.0f);
        SceneCamera camera(view_matrix, proj_matrix);

        context.SetViewProjection(ViewProjection {
            .view_matrix = camera.GetViewMatrix(),
            .projection_matrix = camera.GetProjectionMatrix(),
        });

        for (auto &&system : m_RenderSystems)
            system->Render(context.Buffer(), m_EntityRegistry);
    }

    void Scene::Reset() {
        m_UpdateSystems.clear();
        m_RenderSystems.clear();
    }
}
