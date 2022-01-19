#pragma once

#include <VX/Noncopyable.h>
#include <VX/Core/NativeWindow.h>

namespace VX::Core {
    class Window final {
        VX_MAKE_NONMOVABLE(Window);
        VX_MAKE_NONCOPYABLE(Window);
    private:
        NativeWindow m_native_window;

    public:
        explicit Window(NativeWindow window)
            : m_native_window(window)
        {};

        ~Window() = default;

        [[nodiscard]] NativeWindow native_window() const { return m_native_window; }
    };
}
