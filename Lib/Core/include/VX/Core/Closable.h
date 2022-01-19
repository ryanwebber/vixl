#pragma once

#include <functional>

#include <VX/Noncopyable.h>

namespace VX::Core {
    class Closable {
        VX_MAKE_NONCOPYABLE(Closable);
    private:
        std::function<void(void)> m_CloseFn;
    public:
        explicit Closable(std::function<void(void)> close_fn);
        Closable(Closable &&) noexcept;
        Closable &operator=(Closable &&) noexcept;
        ~Closable();
    };
}
