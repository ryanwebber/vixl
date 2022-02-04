#include <vector>

#include <VX/Core/Material.h>
#include <VX/Core/RenderBuiltins.h>
#include <VX/Core/Shader.h>

#include <VX/Core/Generated/Assets/builtins.h>

namespace VX::Core {

    RenderBuiltins::RenderBuiltins(AssetBundle &bundle)
        : m_asset_bundle(std::move(bundle))
    {
        VX_ASSERT(m_asset_bundle.buffer().size() == VX::Core::Generated::Assets::Builtins::file_size, "Asset bundle provided to render builtins is the wrong size");
    }

    void RenderBuiltins::ensure_initialized() {

#warning "RenderBuiltins::ensure_initialized not implemented"

        // Load shapes
        m_shapes.push_back({ });

        // Load textures
        m_textures.emplace_back(nullptr);

        // Load materials
        m_materials.emplace_back("Sprite", nullptr, nullptr);
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
