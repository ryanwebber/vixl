#pragma once

#include <Common/Noncopyable.h>
#include <Core/NativeWindow.h>

namespace Core {
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
