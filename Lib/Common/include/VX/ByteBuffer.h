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
        std::unique_ptr<std::byte[]> m_buffer;
        size_t m_size;

    public:
        ByteBuffer(std::unique_ptr<std::byte[]> buffer, size_t size)
                : m_buffer(std::move(buffer))
                , m_size(size)
        {}

        ~ByteBuffer() = default;

        [[nodiscard]] size_t size() const { return m_size; }
        [[nodiscard]] std::span<std::byte> view() const { return std::span { m_buffer.get(), m_size }; }
        [[nodiscard]] ByteBuffer get_owned_slice(size_t offset, size_t size) const {
            if (offset + size >= m_size) {
                // Crash?
            }
            auto buffer = std::make_unique<std::byte[]>(size);
            memcpy(buffer.get(), &m_buffer[offset], size);
            return { std::move(buffer), size };
        }
    };
}
