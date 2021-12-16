#pragma once

#include <Common/Copyable.h>
#include <Common/Expected.h>

#include <Core/Size.h>

struct GLFWwindow;

namespace Core {
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

        static Common::Expected<NativeWindow> Create(SizeInt window_size);
    };
}
