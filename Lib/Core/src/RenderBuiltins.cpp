#include <vector>
#include <bgfx/bgfx.h>

#include <VX/Core/Material.h>
#include <VX/Core/RenderBuiltins.h>
#include <VX/Core/Shader.h>

#include <VX/Core/Generated/Assets/builtins.h>

#include <VX/Core/Generated/Shader/Sprite/fragment.h>
#include <VX/Core/Generated/Shader/Sprite/vertex.h>

namespace VX::Core {
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

    Shape create_texture_quad() {
        static UVCoordinate coordinates[] =
            {
                {{ 0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f }},
                {{ 0.5f,  -0.5f, 0.0f }, { 1.0f, 1.0f }},
                {{ -0.5f,  -0.5f, 0.0f }, { 0.0f, 1.0f }},
                {{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }},
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

    RenderBuiltins::RenderBuiltins(AssetBundle &bundle)
        : m_asset_bundle(std::move(bundle))
    {
        VX_ASSERT(m_asset_bundle.buffer().size() == VX::Core::Generated::Assets::Builtins::file_size, "Asset bundle provided to render builtins is the wrong size");
    }

    void RenderBuiltins::ensure_initialized() {
        // Load shapes
        m_shapes.push_back(create_texture_quad());

        // Load textures
        auto logo_default_texture_offset = VX::Core::Generated::Assets::Builtins::logo_default::asset.offset;
        auto logo_default_texture_size = VX::Core::Generated::Assets::Builtins::logo_default::asset.size;
        auto logo_default_texture_data = m_asset_bundle.buffer().get_slice(logo_default_texture_offset, logo_default_texture_size);
        auto logo_default_texture = Texture::create(logo_default_texture_data, BGFX_SAMPLER_POINT);
        m_textures.push_back(logo_default_texture.value());

        // Load materials
        auto sprite_vs_handle = make_shader({ vx_sprite_vertex_shader, sizeof(vx_sprite_vertex_shader) });
        auto sprite_fs_handle = make_shader({ vx_sprite_fragment_shader, sizeof(vx_sprite_fragment_shader) });
        m_materials.emplace_back("Sprite", std::move(sprite_vs_handle), std::move(sprite_fs_handle));
    }

    Promise<std::shared_ptr<RenderBuiltins>> RenderBuiltins::load(ResourceManager &resource_manager, const std::filesystem::path &path) {
        auto file_size = VX::Core::Generated::Assets::Builtins::file_size;
        auto promise = resource_manager.load_asset_bundle(path, 0 , file_size)
                .map<std::shared_ptr<RenderBuiltins>>([](auto& bundle) {
                    return std::shared_ptr<RenderBuiltins>(new RenderBuiltins(bundle));
                });

        return std::move(promise);
    }

    std::shared_ptr<RenderBuiltins> RenderBuiltins::load_sync(const ResourceLocator &resource_locator, const std::filesystem::path &path) {
        std::shared_ptr<VX::Core::RenderBuiltins> builtins = nullptr;
        VX::Core::EventLoop::run_scoped([&](auto executor) -> VX::Core::Closable {
            VX::Core::ResourceManager resource_manager(executor, resource_locator);
            auto load_builtins = VX::Core::RenderBuiltins::load(resource_manager, path)
                    .finally([&](auto &loaded_builtins) {
                        builtins = std::move(loaded_builtins);
                    });

            return std::move(load_builtins);
        });

        VX_ASSERT(builtins != nullptr, "RenderBuiltins not loaded but temporary event loop has exited. Possible async bug.");

        return builtins;
    }
}
