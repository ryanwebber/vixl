#include <Core/Closable.h>

namespace Core {
    Closable::Closable(std::function<void(void)> close_fn)
        : m_CloseFn(std::move(close_fn))
        {
        }

    Closable::Closable(Closable &&other) noexcept
        : m_CloseFn(std::move(other.m_CloseFn))
        {
        }

    Closable &Closable::operator=(Closable &&other) noexcept {
        m_CloseFn = std::move(other.m_CloseFn);
        return *this;
    }

    Closable::~Closable() {
        if (m_CloseFn != nullptr) {
            m_CloseFn();
        }
    }
}
