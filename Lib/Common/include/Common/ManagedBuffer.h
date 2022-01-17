#pragma once

#include <memory>

#include "Copyable.h"
#include "Noncopyable.h"

namespace Common {
    template <typename T>
    class ManagedBuffer final {
        VX_MAKE_NONCOPYABLE(ManagedBuffer);
        VX_DEFAULT_MOVABLE(ManagedBuffer);

    private:
        size_t m_Size { 0 };
        std::unique_ptr<T> m_Buffer;

    public:
        ManagedBuffer() = default;
        ManagedBuffer(std::unique_ptr<T> data, size_t size)
            : m_Buffer(std::move(data))
            , m_Size(size)
        {}

        ~ManagedBuffer() = default;

        [[nodiscard]] auto GetData() const { return &m_Buffer.get()[0]; };
        [[nodiscard]] size_t GetSize() const { return m_Size; };

        auto operator*() const { return &m_Buffer.get()[0]; }
    };
}
