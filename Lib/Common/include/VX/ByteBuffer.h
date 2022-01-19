#pragma once

#include <memory>
#include <span>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

namespace VX {
    class ByteBuffer final {
        VX_MAKE_NONCOPYABLE(ByteBuffer);
        VX_DEFAULT_MOVABLE(ByteBuffer);
    private:
        std::unique_ptr<std::byte[]> m_Buffer;
        size_t m_Size;

    public:
        ByteBuffer(std::unique_ptr<std::byte[]> buffer, size_t size)
                : m_Buffer(std::move(buffer))
                , m_Size(size)
        {}

        ~ByteBuffer() = default;

        [[nodiscard]] size_t GetSize() const { return m_Size; }
        [[nodiscard]] std::span<std::byte> GetView() const { return std::span { m_Buffer.get(), m_Size }; }
        [[nodiscard]] ByteBuffer GetOwnedSlice(size_t offset, size_t size) const {
            if (offset + size >= m_Size) {
                // Crash?
            }
            auto buffer = std::make_unique<std::byte[]>(size);
            memcpy(buffer.get(), &m_Buffer[offset], size);
            return { std::move(buffer), size };
        }
    };
}
