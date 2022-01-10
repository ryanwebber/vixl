#include <bgfx/bgfx.h>

#include <Core/DemoRenderSystem.h>
#include <Core/Logger.h>
#include <Core/Shader.h>

#include <Core/Generated/Shader/uvtex/fragment.h>
#include <Core/Generated/Shader/uvtex/vertex.h>

static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
};

static uint16_t indexes[] = {
        0, 1, 2,
};

static const float cubeVertices[] =
{
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
};

static const uint16_t cubeTriList[] =
{
        0, 1, 2,
        1, 3, 2,
        4, 6, 5,
        5, 6, 7,
        0, 2, 4,
        4, 2, 6,
        1, 5, 3,
        5, 7, 3,
        0, 4, 1,
        4, 5, 1,
        2, 3, 6,
        6, 3, 7,
};

namespace Core {
    DemoRenderSystem::DemoRenderSystem() {
        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

        m_VertexBuffer = std::make_shared<VertexBuffer>(vb);
        m_IndexBuffer = std::make_shared<IndexBuffer>(ib);
        m_Material = VX_CREATE_MATERIAL("UVTexture", uvtex);
    }

    void DemoRenderSystem::Render(RenderTarget &target, const SceneCamera &camera) {
        RenderCommand command(m_VertexBuffer, m_IndexBuffer, m_Material);
        target.Commands().push_back(command);
    }
}
