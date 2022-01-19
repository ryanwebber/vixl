#pragma once

#include <VX/Core/NativeWindow.h>

namespace VX::Core {

    enum class ControlState { None, Terminate };

    class Input final {
    private:
        NativeWindow m_native_window;

    public:
        explicit Input(NativeWindow window)
            : m_native_window(window)
            {};

        ~Input() = default;

        ControlState process_events();
    };
}
