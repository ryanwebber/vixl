#pragma once

#include <memory>

#include "Copyable.h"

namespace Common {

// Manages a buffer by holding onto a type-erased shared ptr to the owning
// system. The shared_ptr keeps the parent from deallocating the buffer
// while this object lives. Deallocating this object doesn't deallocate the
// buffer, the object captured by the shared_ptr should do that in it's
// destructor. Moving or copying this object is an effective move or copy
// of the owning shared_ptr.
    template<typename T>
    class UnmanagedBuffer final {
    VX_DEFAULT_COPYABLE(UnmanagedBuffer);

    VX_DEFAULT_MOVABLE(UnmanagedBuffer);

    private:
        std::shared_ptr<void> m_Owner;
        T *m_Buffer;
        size_t m_Length{0};

    public:
        UnmanagedBuffer()
                : m_Owner({}), m_Buffer(nullptr), m_Length(0) {};

        explicit UnmanagedBuffer(std::shared_ptr<void> owner, T *buffer, size_t length)
                : m_Owner(std::move(owner)), m_Buffer(buffer), m_Length(length) {};

        ~UnmanagedBuffer() = default;

        T *GetData() { return m_Buffer; };

        const T *GetData() const { return m_Buffer; };

        [[nodiscard]] size_t GetLength() const { return m_Length; };

        T *operator->() { return &m_Buffer; }

        const T *operator->() const { return &m_Buffer; }
    };
}