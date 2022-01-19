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
        GLFWwindow* m_Window;
        explicit NativeWindow(GLFWwindow* window)
                : m_Window(window)
        {}

    public:

        ~NativeWindow() = default;

        [[nodiscard]] GLFWwindow* GetWindowPointer() const { return m_Window; }

        [[nodiscard]] void* GetPlatformWindowHandle() const;
        [[nodiscard]] void* GetPlatformDisplayType() const;
        [[nodiscard]] SizeInt GetSize() const;

        void Destroy();
        void SwapBuffers();

        static VX::Expected<NativeWindow> Create(SizeInt window_size);
    };
}
