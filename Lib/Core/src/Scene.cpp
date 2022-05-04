#include <VX/Core/Scene.h>

namespace VX::Core {
    void Scene::configure() {
        if (m_configured)
            return;

        m_configured = true;

        auto weak_self = weak_from_this();
        for (auto &&system : m_meta_systems)
            system->configure(weak_self);
    }

    void Scene::update() {
        for (auto &&system : m_update_systems)
            system->update(m_entity_registry);
    }

    void Scene::render(const SceneContext& scene_context, const RenderContext &render_context, RenderPass &render_pass) {
        for (auto &&system : m_render_systems)
            system->render(scene_context, render_context, render_pass, m_entity_registry);
    }

    void Scene::reset() {
        m_update_systems.clear();
        m_render_systems.clear();
    }
}
