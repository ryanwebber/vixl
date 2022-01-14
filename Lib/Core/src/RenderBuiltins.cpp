#include <bgfx/bgfx.h>
#include <Core/RenderBuiltins.h>

#include <Core/Generated/Assets/builtins.h>

namespace Core {
    struct Vec3 {
        float x, y, z;
    };

    struct UV {
        float u, v;
    };

    struct UVCoordinate {
        Vec3 position;
        UV uv;
    };

    Shape CreateTextureQuad() {
        static UVCoordinate coordinates[] =
            {
                {{ 0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f }},
                {{ 0.5f,  -0.5f, 0.0f }, { 1.0f, 0.0f }},
                {{ -0.5f,  -0.5f, 0.0f }, { 0.0f, 0.0f }},
                {{ -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f }},
            };

        static const uint16_t indexes[] =
            {
                3, 1, 0,
                3, 2, 1
            };

        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(coordinates, sizeof(coordinates)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(indexes, sizeof(indexes)));

        return {
            .vertex_buffer = std::move(std::make_shared<VertexBufferHandle>(vb)),
            .index_buffer = std::move(std::make_shared<IndexBufferHandle>(ib))
        };
    }

    RenderBuiltins::RenderBuiltins()
        : m_TextureQuad(CreateTextureQuad())
        {
        }
}
