#pragma once

#include <memory>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include <VX/Noncopyable.h>
#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderSystem.h>
#include <VX/Core/RenderTarget.h>
#include <VX/Core/UpdateSystem.h>
#include <VX/Core/MetaSystem.h>

namespace VX::Core {
class Scene final : public std::enable_shared_from_this<Scene> {
        VX_MAKE_NONMOVABLE(Scene);
        VX_MAKE_NONCOPYABLE(Scene);
    private:
        std::string m_name;

        entt::registry m_entity_registry;
        std::vector<std::shared_ptr<UpdateSystem>> m_update_systems;
        std::vector<std::shared_ptr<RenderSystem>> m_render_systems;
        std::vector<std::shared_ptr<MetaSystem>> m_meta_systems;

        explicit Scene(const std::string_view &name)
            : m_name(name)
            { }

    public:

        ~Scene() = default;

        void configure();
        void update();
        void render(RenderContext &context, const RenderTarget &target);

        [[nodiscard]] const std::string &name() const { return m_name; }

        entt::registry& entities() { return m_entity_registry; }
        [[nodiscard]] const entt::registry& entities() const { return m_entity_registry; }

        std::vector<std::shared_ptr<UpdateSystem>>& update_systems() { return m_update_systems; }
        [[nodiscard]] const std::vector<std::shared_ptr<UpdateSystem>>& update_systems() const { return m_update_systems; }

        std::vector<std::shared_ptr<RenderSystem>>& render_systems() { return m_render_systems; }
        [[nodiscard]] const std::vector<std::shared_ptr<RenderSystem>>& render_systems() const { return m_render_systems; }

        std::vector<std::shared_ptr<MetaSystem>>& meta_systems() { return m_meta_systems; }
        [[nodiscard]] const std::vector<std::shared_ptr<MetaSystem>>& meta_systems() const { return m_meta_systems; }

        void reset();

        static std::shared_ptr<Scene> create_named(const std::string_view &name) {
            return std::shared_ptr<Scene>(new Scene(name));
        }
    };
}
