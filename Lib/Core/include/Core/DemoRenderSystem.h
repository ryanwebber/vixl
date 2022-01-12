#pragma once

#include <Core/RenderSystem.h>
#include <Core/Types.h>

namespace Core {
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
