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
    DemoRenderSystem::DemoRenderSystem(const RenderBuiltins& builtins) {

        // Load a vertex and index buffer for our simple triangle
        bgfx::VertexLayout vertex_layout;
        vertex_layout
            .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        auto vb = bgfx::createVertexBuffer(bgfx::makeRef(vertexes, sizeof(vertexes)), vertex_layout);
        auto ib =  bgfx::createIndexBuffer(bgfx::makeRef(indexes, sizeof(indexes)));

        m_triangle_vertex_buffer = std::make_shared<VertexBufferHandle>(vb);
        m_triangle_index_buffer = std::make_shared<IndexBufferHandle>(ib);

        auto colored_vs_handle = make_shader({ colored_vertex_shader, sizeof(colored_vertex_shader) });
        auto colored_fs_handle = make_shader({ colored_fragment_shader, sizeof(colored_fragment_shader) });
        m_triangle_material = std::make_shared<Material>("Colored", std::move(colored_vs_handle), std::move(colored_fs_handle));

        // Load the material for our primitive quad
        auto uvs_vs_handle = make_shader({ uvmap_vertex_shader, sizeof(uvmap_vertex_shader) });
        auto uvs_fs_handle = make_shader({ uvmap_fragment_shader, sizeof(uvmap_fragment_shader) });
        m_quad_material = std::make_shared<Material>("UVMap", std::move(uvs_vs_handle), std::move(uvs_fs_handle));

        auto texture_handle = builtins.get_texture(Textures::UVMap).texture_handle();
        auto uniform_handle = std::make_shared<UniformHandle>(bgfx::createUniform("s_texColor",  bgfx::UniformType::Sampler));
        m_sprite_material = std::make_shared<Material>(builtins.get_material(Materials::Sprite).clone());
        m_sprite_material->set_texture<0>({ uniform_handle, texture_handle });
    }

    static int counter = 0;
    void DemoRenderSystem::render(RenderBuffer &buffer, entt::registry&) {
        counter++;

        float x = cos((float)counter * 0.025f);
        float y = sin((float)counter * 0.025f);

        auto triangle_transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(x + 4, y, 0.0f));
        buffer.draw_indexed(triangle_transform, m_triangle_vertex_buffer, m_triangle_index_buffer, m_triangle_material);

        auto quad_transform = glm::translate(glm::mat4x4(1.0f), glm::vec3(x - 4, -y, 0.0f));
        buffer.draw_texture_quad(quad_transform, m_quad_material);

        auto quad_transform2 = glm::scale(glm::mat4x4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
        buffer.draw_texture_quad(quad_transform2, m_sprite_material);
    }
}
