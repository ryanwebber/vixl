#pragma once

class SDL_Window;
class SDL_SysWMinfo;

namespace VX::Platform {

    struct Viewport {
        int width;
        int height;
    };

    class NativeWindow {
    private:
        SDL_Window* m_sdl_window;
    public:
        explicit NativeWindow(SDL_Window* window)
            : m_sdl_window(window)
        {}

        ~NativeWindow();

        [[nodiscard]] SDL_Window* sdl_window() const { return m_sdl_window; }
        [[nodiscard]] SDL_SysWMinfo sdl_window_info() const;

        Viewport viewport() const;
    };
}
