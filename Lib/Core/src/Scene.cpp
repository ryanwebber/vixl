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

    void Scene::render(const RenderContext &context, RenderBuffer &buffer) {
        for (auto &&system : m_render_systems)
            system->render(context, buffer, m_entity_registry);
    }

    void Scene::reset() {
        m_update_systems.clear();
        m_render_systems.clear();
    }
}
