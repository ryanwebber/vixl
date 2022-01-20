#include <vector>
#include <bgfx/bgfx.h>

#include <VX/Core/RenderBuiltins.h>
#include <VX/Core/Generated/Assets/builtins.h>

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

    RenderBuiltins::RenderBuiltins(AssetBundle &bundle)
        : m_asset_bundle(std::move(bundle))
    {
    }

    void RenderBuiltins::ensure_initialized() {
        m_shapes.push_back(create_texture_quad());
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
        VX::Core::EventLoop::run_scoped([&](auto executor) -> std::vector<VX::Core::Closable> {
            VX::Core::ResourceManager resource_manager(executor, resource_locator);
            std::vector<VX::Core::Closable> handles;

            auto load_builtins = VX::Core::RenderBuiltins::load(resource_manager, path)
                    .finally([&](auto &loaded_builtins) {
                        builtins = std::move(loaded_builtins);
                    });

            handles.push_back(std::move(load_builtins));
            return handles;
        });

        return builtins;
    }
}
