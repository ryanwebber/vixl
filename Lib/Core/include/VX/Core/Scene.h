#pragma once

#include <memory>
#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

#include <VX/Noncopyable.h>
#include <VX/Weakable.h>
#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderSystem.h>
#include <VX/Core/UpdateSystem.h>
#include <VX/Core/MetaSystem.h>

namespace VX::Core {
class Scene final : public VX::Weakable<Scene> {
        VX_MAKE_NONMOVABLE(Scene);
        VX_MAKE_NONCOPYABLE(Scene);
    private:
        std::string m_Name;

        entt::registry m_EntityRegistry;
        std::vector<std::shared_ptr<UpdateSystem>> m_UpdateSystems;
        std::vector<std::shared_ptr<RenderSystem>> m_RenderSystems;
        std::vector<std::shared_ptr<MetaSystem>> m_MetaSystems;

        explicit Scene(const std::string_view &name)
            : m_Name(name)
            { }

    public:

        ~Scene() = default;

        void Configure();
        void Update();
        void Render(RenderContext &context);

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

        std::vector<std::shared_ptr<MetaSystem>>& MetaSystems() { return m_MetaSystems; }
        [[nodiscard]] const std::vector<std::shared_ptr<MetaSystem>>& GetMetaSystems() const { return m_MetaSystems; }
        void SetMetaSystems(std::vector<std::shared_ptr<MetaSystem>> systems) {
            m_MetaSystems = std::move(systems);
        }

        void Reset();

        static std::shared_ptr<Scene> Create(const std::string_view &name) {
            return std::shared_ptr<Scene>(new Scene(name));
        }
    };
}
