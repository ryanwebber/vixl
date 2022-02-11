#pragma once

#include <vector>
#include <glm/glm.hpp>

class SDL_Window;
class SDL_SysWMinfo;
class SDL_Renderer;

namespace VX::Platform {
    class NativeWindow {
    private:
        SDL_Window* m_sdl_window;
        std::vector<SDL_Renderer*> m_renderers;
    public:
        explicit NativeWindow(SDL_Window* window)
            : m_sdl_window(window)
        {}

        ~NativeWindow();

        [[nodiscard]] SDL_Window* sdl_window() const { return m_sdl_window; }
        [[nodiscard]] SDL_SysWMinfo sdl_window_info() const;

        [[nodiscard]] glm::vec2 screen_size() const;

        SDL_Renderer *create_renderer();
    };
}
