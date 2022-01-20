#pragma once

#include <memory>
#include <filesystem>

#include <VX/Core/Async.h>
#include <VX/Core/Types.h>
#include <VX/Core/ResourceManager.h>
#include <VX/Core/Texture.h>

namespace VX::Core {
    struct Shape {
        std::shared_ptr<VertexBufferHandle> vertex_buffer;
        std::shared_ptr<IndexBufferHandle> index_buffer;
    };

    enum class Textures : size_t {
        UVMap = 0
    };

    enum class Shapes : size_t {
        Quad = 0
    };

    class RenderBuiltins final {
    private:
        AssetBundle m_asset_bundle;
        std::vector<Shape> m_shapes;
        std::vector<Texture> m_textures;

    public:
        explicit RenderBuiltins(AssetBundle&);
        ~RenderBuiltins() = default;

        void ensure_initialized();

        [[nodiscard]] const Shape& get_shape(Shapes shape) const {
            return m_shapes[static_cast<size_t>(shape)];
        }

        [[nodiscard]] const Texture& get_texture(Textures texture) const {
            return m_textures[static_cast<size_t>(texture)];
        }

        static Promise<std::shared_ptr<RenderBuiltins>> load(ResourceManager& resource_manager, const std::filesystem::path &path);
        static std::shared_ptr<RenderBuiltins> load_sync(const ResourceLocator &resource_locator, const std::filesystem::path &path);
    };
}
