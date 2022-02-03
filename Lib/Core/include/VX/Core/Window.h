#pragma once

#include <memory>

#include <VX/Noncopyable.h>
#include <VX/Platform/NativeWindow.h>

namespace VX::Core {

    using namespace VX::Platform;

    class Window final {
        VX_MAKE_NONMOVABLE(Window);
        VX_MAKE_NONCOPYABLE(Window);
    private:
        std::shared_ptr<NativeWindow> m_native_window;

    public:
        explicit Window(std::shared_ptr<NativeWindow> native_window)
            : m_native_window(std::move(native_window))
        {};

        ~Window() = default;

        [[nodiscard]] const NativeWindow& native_window() const { return *m_native_window; }
    };
}
