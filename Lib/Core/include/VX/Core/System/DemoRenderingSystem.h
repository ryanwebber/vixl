#pragma once

#include <VX/Noncopyable.h>

#include <VX/Core/RenderBuiltins.h>
#include <VX/Core/RenderSystem.h>
#include <VX/Core/Types.h>

namespace VX::Core::System {
    class DemoRenderingSystem final: public RenderSystem {
        VX_MAKE_NONCOPYABLE(DemoRenderingSystem);
        VX_MAKE_NONMOVABLE(DemoRenderingSystem);
    private:
        std::shared_ptr<VertexBufferHandle> m_triangle_vertex_buffer;
        std::shared_ptr<IndexBufferHandle> m_triangle_index_buffer;
        std::shared_ptr<Material> m_triangle_material;
        std::shared_ptr<Material> m_quad_material;
        std::shared_ptr<Material> m_sprite_material;

    public:
        explicit DemoRenderingSystem(const RenderBuiltins&);
        ~DemoRenderingSystem() override = default;

        void render(RenderBuffer&, entt::registry& entities) override;
    };
}
