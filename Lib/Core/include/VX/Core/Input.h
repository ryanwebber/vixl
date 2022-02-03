#pragma once

#include <memory>
#include <unordered_map>

#include <VX/Core/Async.h>
#include <VX/Core/Event.h>
#include <VX/Platform/NativeWindow.h>

namespace VX::Core {

    using namespace VX::Platform;

    enum class ControlState { None, Terminate };

    class Input final {
    private:
        std::shared_ptr<NativeWindow> m_native_window;

        // Event handlers
        EventEmitter<WindowResizeEvent> m_on_window_resize_event;

    public:
        Input(std::shared_ptr<NativeWindow> window, Executor &executor);

        ~Input() = default;

        ControlState process_events();

        // Event handlers
        Subscriber<WindowResizeEvent> &on_window_resize() { return m_on_window_resize_event.subscriber(); }
    };
}
