#pragma once

#include <VX/Core/RenderSystem.h>
#include <VX/Core/Types.h>

namespace VX::Core {
    class DemoRenderSystem final: public RenderSystem {
    private:
        std::shared_ptr<VertexBufferHandle> m_triangle_vertex_buffer;
        std::shared_ptr<IndexBufferHandle> m_triangle_index_buffer;
        std::shared_ptr<Material> m_triangle_material;
        std::shared_ptr<Material> m_quad_material;

    public:
        DemoRenderSystem();
        ~DemoRenderSystem() override = default;

        void render(RenderBuffer&, entt::registry& entities) override;
    };
}
