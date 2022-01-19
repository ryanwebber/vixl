#include <VX/Core/Closable.h>
#include <VX/Core/Logger.h>

namespace VX::Core {
    Closable::Closable(std::function<void(void)> close_fn)
        : m_close_fn(std::move(close_fn))
    {
    }

    Closable::Closable(Closable &&other) noexcept
        : m_close_fn(std::move(other.m_close_fn))
    {
        other.m_close_fn = nullptr;
    }

    Closable &Closable::operator=(Closable &&other) noexcept {
        m_close_fn = std::move(other.m_close_fn);
        other.m_close_fn = nullptr;
        return *this;
    }

    Closable::~Closable() {
        if (m_close_fn != nullptr) {
            m_close_fn();
        }
    }
}
