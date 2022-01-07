#pragma once

#include <memory>
#include <string>
#include <vector>

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
        std::vector<std::shared_ptr<UpdateSystem>> m_UpdateSystems;
        std::vector<std::shared_ptr<RenderSystem>> m_RenderSystems;

    public:
        explicit Scene(const std::string_view &name)
            : m_Name(name)
            { }

        ~Scene() = default;

        void Update();
        void Render(RenderTarget &target);

        std::vector<std::shared_ptr<UpdateSystem>>& UpdateSystems() { return m_UpdateSystems; }
        [[nodiscard]] const std::vector<std::shared_ptr<UpdateSystem>>& UpdateSystems() const { return m_UpdateSystems; }
        void SetUpdateSystems(std::vector<std::shared_ptr<UpdateSystem>> systems) {
            m_UpdateSystems = std::move(systems);
        }

        std::vector<std::shared_ptr<RenderSystem>>& RenderSystems() { return m_RenderSystems; }
        [[nodiscard]] const std::vector<std::shared_ptr<RenderSystem>>& RenderSystems() const { return m_RenderSystems; }
        void SetRenderSystems(std::vector<std::shared_ptr<RenderSystem>> systems) {
            m_RenderSystems = std::move(systems);
        }

        void Reset();
    };
}
