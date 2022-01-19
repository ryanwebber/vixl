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
        VX::ByteBuffer m_Buffer;

    public:
        AssetBundle(VX::ByteBuffer buffer)
            : m_Buffer(std::move(buffer))
        {}

        ~AssetBundle() = default;

        const VX::ByteBuffer& GetBuffer() const { return m_Buffer; }

        VX::ByteBuffer GetOwnedAsset(const PackedAsset& definition) const {
            return m_Buffer.GetOwnedSlice(definition.offset, definition.size);
        }
    };
}
