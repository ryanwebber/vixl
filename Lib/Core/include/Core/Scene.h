#pragma once

#include <memory>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include <Common/Noncopyable.h>
#include <Core/RenderTarget.h>
#include <Core/RenderSystem.h>
#include <Core/UpdateSystem.h>

namespace Core {
    class Scene final {
        VX_MAKE_NONMOVABLE(Scene);
        VX_MAKE_NONCOPYABLE(Scene);
    private:
        std::string m_Name;

        entt::registry m_EntityRegistry;
        std::vector<std::shared_ptr<UpdateSystem>> m_UpdateSystems;
        std::vector<std::shared_ptr<RenderSystem>> m_RenderSystems;

    public:
        explicit Scene(const std::string_view &name)
            : m_Name(name)
            { }

        ~Scene() = default;

        void Update();
        void Render(RenderTarget &target);

        [[nodiscard]] const std::string &GetName() const { return m_Name; }

        entt::registry& Entities() { return m_EntityRegistry; }
        [[nodiscard]] const entt::registry& GetEntities() const { return m_EntityRegistry; }

        std::vector<std::shared_ptr<UpdateSystem>>& UpdateSystems() { return m_UpdateSystems; }
        [[nodiscard]] const std::vector<std::shared_ptr<UpdateSystem>>& GetUpdateSystems() const { return m_UpdateSystems; }
        void SetUpdateSystems(std::vector<std::shared_ptr<UpdateSystem>> systems) {
            m_UpdateSystems = std::move(systems);
        }

        std::vector<std::shared_ptr<RenderSystem>>& RenderSystems() { return m_RenderSystems; }
        [[nodiscard]] const std::vector<std::shared_ptr<RenderSystem>>& GetRenderSystems() const { return m_RenderSystems; }
        void SetRenderSystems(std::vector<std::shared_ptr<RenderSystem>> systems) {
            m_RenderSystems = std::move(systems);
        }

        void Reset();
    };
}
