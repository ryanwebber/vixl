#include <VX/Core/Closable.h>
#include <VX/Core/Logger.h>

namespace VX::Core {
    Closable::Closable(std::function<void(void)> close_fn)
        : m_CloseFn(std::move(close_fn))
    {
    }

    Closable::Closable(Closable &&other) noexcept
        : m_CloseFn(std::move(other.m_CloseFn))
    {
        other.m_CloseFn = nullptr;
    }

    Closable &Closable::operator=(Closable &&other) noexcept {
        m_CloseFn = std::move(other.m_CloseFn);
        other.m_CloseFn = nullptr;
        return *this;
    }

    Closable::~Closable() {
        if (m_CloseFn != nullptr) {
            m_CloseFn();
        }
    }
}
