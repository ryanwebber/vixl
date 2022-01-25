#pragma once

#include <unordered_map>

#include <VX/Core/Async.h>
#include <VX/Core/Event.h>
#include <VX/Core/NativeWindow.h>

namespace VX::Core {

    enum class ControlState { None, Terminate };

    class Input final {
    private:
        NativeWindow m_native_window;

        // Event handlers
        EventEmitter<WindowResizeEvent> m_on_window_resize_event;

    public:
        explicit Input(NativeWindow window, Executor &executor);

        ~Input() = default;

        ControlState process_events();

        // Event handlers
        Subscriber<WindowResizeEvent> &on_window_resize() { return m_on_window_resize_event.subscriber(); }
    };
}
