#pragma once

#include <memory>
#include <filesystem>

#include <VX/Core/Async.h>
#include <VX/Core/Types.h>
#include <VX/Core/ResourceManager.h>
#include <VX/Core/Texture.h>
#include "Material.h"

namespace VX::Core {
    struct Shape {
        std::shared_ptr<VertexBufferHandle> vertex_buffer;
        std::shared_ptr<IndexBufferHandle> index_buffer;
    };

    enum class Materials : size_t {
        Sprite = 0
    };

    enum class Textures : size_t {
        LogoDefault = 0
    };

    enum class Shapes : size_t {
        Quad = 0
    };

    class RenderBuiltins final {
    private:
        AssetBundle m_asset_bundle;
        std::vector<Shape> m_shapes;
        std::vector<Texture> m_textures;
        std::vector<Material> m_materials;

    public:
        explicit RenderBuiltins(AssetBundle&);
        ~RenderBuiltins() = default;

        void ensure_initialized();

        [[nodiscard]] const Shape& get_shape(Shapes shape) const {
            auto idx = static_cast<size_t>(shape);
            VX_ASSERT(idx < m_shapes.size(), "Invalid shape. Shapes might not have been loaded yet");
            return m_shapes[idx];
        }

        [[nodiscard]] const Texture& get_texture(Textures texture) const {
            auto idx = static_cast<size_t>(texture);
            VX_ASSERT(idx < m_textures.size(), "Invalid texture. Textures might not have been loaded yet");
            return m_textures[idx];
        }

        [[nodiscard]] const Material& get_material(Materials material) const {
            auto idx = static_cast<size_t>(material);
            VX_ASSERT(idx < m_materials.size(), "Invalid material. Materials might not have been loaded yet");
            return m_materials[idx];
        }

        static Promise<std::shared_ptr<RenderBuiltins>> load(ResourceManager& resource_manager, const std::filesystem::path &path);
        static std::shared_ptr<RenderBuiltins> load_sync(const ResourceLocator &resource_locator, const std::filesystem::path &path);
    };
}
