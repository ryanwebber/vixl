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

    void Scene::render(RenderPass &render_pass) {
        RenderContext context;
        for (auto &&system : m_render_systems)
            system->render(context, render_pass, m_entity_registry);
    }

    void Scene::reset() {
        m_update_systems.clear();
        m_render_systems.clear();
    }
}
