#pragma once

#include <memory>
#include <span>

#include <Common/Copyable.h>
#include <Common/Noncopyable.h>
#include <Common/ByteBuffer.h>

#include <Core/PackedAsset.h>

namespace Core {
    class AssetBundle final {
        VX_MAKE_NONCOPYABLE(AssetBundle);
        VX_DEFAULT_MOVABLE(AssetBundle);
    private:
        Common::ByteBuffer m_Buffer;

    public:
        AssetBundle(Common::ByteBuffer buffer)
            : m_Buffer(std::move(buffer))
        {}

        ~AssetBundle() = default;

        const Common::ByteBuffer& GetBuffer() const { return m_Buffer; }

        Common::ByteBuffer GetOwnedAsset(const PackedAsset& definition) const {
            return m_Buffer.GetOwnedSlice(definition.offset, definition.size);
        }
    };
}
