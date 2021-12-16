#pragma once

#include <Core/NativeWindow.h>

namespace Core {

    enum class ControlState { None, Terminate };

    class Input final {
    private:
        NativeWindow m_NativeWindow;

    public:
        explicit Input(NativeWindow window)
            : m_NativeWindow(window)
            {};

        ~Input() = default;

        ControlState ProcessEvents();
    };
}
