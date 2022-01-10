#pragma once

#include <Core/RenderSystem.h>
#include <Core/Types.h>

namespace Core {
    class DemoRenderSystem final: public RenderSystem {
    private:
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<Material> m_Material;

    public:
        DemoRenderSystem();
        ~DemoRenderSystem() override = default;

        void Render(RenderBuffer&) override;
    };
}
