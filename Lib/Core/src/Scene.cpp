#include <glm/glm.hpp>
#include <VX/Core/Scene.h>

namespace VX::Core {
    void Scene::configure() {
        auto weak_self = weak_from_this();
        for (auto &&system : m_meta_systems)
            system->configure(weak_self);
    }

    void Scene::update() {
        for (auto &&system : m_update_systems)
            system->update(m_entity_registry);
    }

    void Scene::render(RenderContext &context) {

        auto up_is_down = bgfx::getCaps()->originBottomLeft ? 1.0f : -1.0f;
        glm::vec3 eye(0.0f, 0.0f, -5.0f);
        glm::vec3 center(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, up_is_down, 0.0f);
        auto view_matrix = glm::lookAt(eye, center, up);
        auto proj_matrix = glm::perspective(80.0f, (800.0f / 600.0f), 0.1f, 100.0f);
        SceneCamera camera(view_matrix, proj_matrix);

        context.set_view_projection({
            .view_matrix = camera.view_matrix(),
            .projection_matrix = camera.projection_matrix(),
        });

        for (auto &&system : m_render_systems)
            system->render(context.buffer(), m_entity_registry);
    }

    void Scene::reset() {
        m_update_systems.clear();
        m_render_systems.clear();
    }
}
