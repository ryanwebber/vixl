#pragma once

#include <memory>
#include <span>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>
#include <VX/ByteBuffer.h>

#include <VX/Core/PackedAsset.h>

namespace VX::Core {
    class AssetBundle final {
        VX_MAKE_NONCOPYABLE(AssetBundle);
        VX_DEFAULT_MOVABLE(AssetBundle);
    private:
        VX::ByteBuffer m_buffer;

    public:
        explicit AssetBundle(VX::ByteBuffer buffer)
            : m_buffer(std::move(buffer))
        {}

        ~AssetBundle() = default;

        [[nodiscard]] const VX::ByteBuffer& buffer() const { return m_buffer; }
        [[nodiscard]] VX::ByteBuffer get_owned_asset(const PackedAsset& definition) const {
            return m_buffer.get_owned_slice(definition.offset, definition.size);
        }
    };
}
