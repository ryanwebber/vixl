#pragma once

#include <VX/Noncopyable.h>
#include <VX/Core/NativeWindow.h>

namespace VX::Core {
    class Window final {
        VX_MAKE_NONMOVABLE(Window);
        VX_MAKE_NONCOPYABLE(Window);
    private:
        NativeWindow m_NativeWindow;

    public:
        explicit Window(NativeWindow window)
            : m_NativeWindow(window)
            {};

        [[nodiscard]] NativeWindow GetNativeWindow() const { return m_NativeWindow; }

        ~Window() = default;
    };
}
