#pragma once

#include <memory>
#include <filesystem>

#include <VX/Core/Async.h>
#include <VX/Core/Types.h>
#include <VX/Core/ResourceManager.h>

namespace VX::Core {
    struct Shape {
        std::shared_ptr<VertexBufferHandle> vertex_buffer;
        std::shared_ptr<IndexBufferHandle> index_buffer;
    };

    enum class Shapes:size_t {
        Quad = 0
    };

    class RenderBuiltins final {
    private:
        AssetBundle m_asset_bundle;
        std::vector<Shape> m_shapes;

        explicit RenderBuiltins(AssetBundle&);

    public:
        ~RenderBuiltins() = default;

        void ensure_initialized();

        [[nodiscard]] const Shape& get_shape(Shapes shape) const {
            return m_shapes[static_cast<size_t>(shape)];
        }

         static Promise<std::shared_ptr<RenderBuiltins>> load(ResourceManager& resource_manager, const std::filesystem::path &path);
    };
}
