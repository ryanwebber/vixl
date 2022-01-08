#include <bgfx/bgfx.h>
#include <Core/DemoRenderSystem.h>

namespace Core {
    DemoRenderSystem::DemoRenderSystem() {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };

        uint16_t indexes[] = {
            0, 1, 3,
        };

        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(indexes, sizeof(indexes)));

        // TODO compile shader to create a material
#error Work in progress
    }

    void DemoRenderSystem::Render(RenderTarget &target, const SceneCamera &camera) {
    }
}
