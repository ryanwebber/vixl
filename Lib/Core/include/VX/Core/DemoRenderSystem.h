#pragma once

#include <VX/Core/RenderSystem.h>
#include <VX/Core/Types.h>

namespace VX::Core {
    class DemoRenderSystem final: public RenderSystem {
    private:
        std::shared_ptr<VertexBufferHandle> m_TriangleVertexBuffer;
        std::shared_ptr<IndexBufferHandle> m_TriangleIndexBuffer;
        std::shared_ptr<Material> m_TriangleMaterial;
        std::shared_ptr<Material> m_QuadMaterial;

    public:
        DemoRenderSystem();
        ~DemoRenderSystem() override = default;

        void Render(RenderBuffer&, entt::registry& entities) override;
    };
}
