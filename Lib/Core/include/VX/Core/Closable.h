#pragma once

#include <functional>

#include <VX/Noncopyable.h>

namespace VX::Core {
    class Closable {
        VX_MAKE_NONCOPYABLE(Closable);
    private:
        std::function<void(void)> m_close_fn;
    public:
        explicit Closable(std::function<void(void)> close_fn);
        Closable(Closable &&) noexcept;
        Closable &operator=(Closable &&) noexcept;
        ~Closable();

        void erase() { m_close_fn = nullptr; }
    };
}
