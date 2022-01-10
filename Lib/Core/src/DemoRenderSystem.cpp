#include <bgfx/bgfx.h>

#include <Core/DemoRenderSystem.h>
#include <Core/Shader.h>

#include <Core/Generated/Shader/uvtex/fragment.h>
#include <Core/Generated/Shader/uvtex/vertex.h>

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex vertexes[] =
    {
        {0.0f,  1.0f,  0.0f, 0xff0000ff },
        { 1.41f,  -1.0f,  0.0f, 0xff00ff00 },
        {-1.41f, -1.0f,  0.0f, 0xffff0000 },
    };

static const uint16_t indexes[] =
{
        // Counter-clockwise ordering
        2,1,0
};

namespace Core {
    DemoRenderSystem::DemoRenderSystem() {
        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(vertexes, sizeof(vertexes)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(indexes, sizeof(indexes)));

        m_VertexBuffer = std::make_shared<VertexBuffer>(vb);
        m_IndexBuffer = std::make_shared<IndexBuffer>(ib);
        m_Material = VX_CREATE_MATERIAL("UVTexture", uvtex);
    }

    void DemoRenderSystem::Render(RenderTarget &target, const SceneCamera &camera) {
        RenderCommand command(m_VertexBuffer, m_IndexBuffer, m_Material);
        target.Commands().push_back(command);
    }
}
