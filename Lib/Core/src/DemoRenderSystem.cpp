#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include <VX/Core/DemoRenderSystem.h>
#include <VX/Core/Shader.h>

#include <VX/Core/Generated/Shader/colored/fragment.h>
#include <VX/Core/Generated/Shader/colored/vertex.h>
#include <VX/Core/Generated/Shader/uvmap/fragment.h>
#include <VX/Core/Generated/Shader/uvmap/vertex.h>

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex vertexes[] =
    {
        {0.0f,  0.5f,  0.0f, 0xfff000ff },
        { 0.5f,  -0.5f,  0.0f, 0xff00fff0 },
        {-0.5f, -0.5f,  0.0f, 0xfffff000 },
    };

static const uint16_t indexes[] =
{
        // Counter-clockwise ordering
        2,1,0
};

namespace VX::Core {
    DemoRenderSystem::DemoRenderSystem() {

        // Load a vertex and index buffer for our simple triangle
        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(vertexes, sizeof(vertexes)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(indexes, sizeof(indexes)));

        m_TriangleVertexBuffer = std::make_shared<VertexBufferHandle>(vb);
        m_TriangleIndexBuffer = std::make_shared<IndexBufferHandle>(ib);
        m_TriangleMaterial = VX_CREATE_MATERIAL("Simple Colored", colored);

        // Load the material for our primative quad
        m_QuadMaterial = VX_CREATE_MATERIAL("UV Map", uvmap);
    }

    static int counter = 0;
    void DemoRenderSystem::Render(RenderBuffer &buffer, entt::registry&) {
        counter++;

        float x = cos((float)counter * 0.025f);
        float y = sin((float)counter * 0.025f);

        auto triangle_transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(x + 2, y, 0.0f));
        buffer.DrawIndexed(triangle_transform, m_TriangleVertexBuffer, m_TriangleIndexBuffer, m_TriangleMaterial);

        auto quad_transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(x - 2, -y, 0.0f));
        buffer.DrawTextureQuad(quad_transform, m_QuadMaterial);
    }
}
