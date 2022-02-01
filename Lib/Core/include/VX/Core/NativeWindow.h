#pragma once

#include <bgfx/bgfx.h>

#include <VX/Copyable.h>
#include <VX/Expected.h>

#include <VX/Core/Size.h>

class SDL_Window;
class SDL_Renderer;

namespace VX::Core {
    class NativeWindow final {
        VX_DEFAULT_MOVABLE(NativeWindow);
        VX_DEFAULT_COPYABLE(NativeWindow);

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bgfx::PlatformData m_platform_data;

        explicit NativeWindow(SDL_Window* window,  SDL_Renderer* renderer, const bgfx::PlatformData &platform_data)
                : m_window(window)
                , m_renderer(renderer)
                , m_platform_data(platform_data)
        {}

    public:

        ~NativeWindow() = default;

        [[nodiscard]] SizeInt size() const;
        [[nodiscard]] const bgfx::PlatformData& platform_data() const { return m_platform_data; }
        [[nodiscard]] std::vector<const char*> graphics_extensions() const;
        [[nodiscard]] void* native_window_pointer() const;

        void destroy();
        void clear();
        void swap_buffers();

        static VX::Expected<NativeWindow> create_with_size(SizeInt window_size);
    };
}
