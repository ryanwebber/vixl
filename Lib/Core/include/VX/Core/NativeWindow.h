#pragma once

#include <VX/Copyable.h>
#include <VX/Expected.h>

#include <VX/Core/Size.h>

struct GLFWwindow;

namespace VX::Core {
    class NativeWindow final {
        VX_DEFAULT_MOVABLE(NativeWindow);
        VX_DEFAULT_COPYABLE(NativeWindow);

    private:
        GLFWwindow* m_window;
        explicit NativeWindow(GLFWwindow* window)
                : m_window(window)
        {}

    public:

        ~NativeWindow() = default;

        [[nodiscard]] GLFWwindow* window_pointer() const { return m_window; }

        [[nodiscard]] void* platform_window_handle() const;
        [[nodiscard]] void* platform_display_type() const;
        [[nodiscard]] SizeInt size() const;

        void destroy();
        void swap_buffers();

        static VX::Expected<NativeWindow> create_with_size(SizeInt window_size);
    };
}
