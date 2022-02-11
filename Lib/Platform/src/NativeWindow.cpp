#include "SDL.h"
#include "SDL_syswm.h"

#include <VX/Platform/NativeWindow.h>

namespace VX::Platform {
    NativeWindow::~NativeWindow()
    {
        for (auto renderer : m_renderers) {
            SDL_DestroyRenderer(renderer);
        }

        SDL_DestroyWindow(m_sdl_window);
    }

    SDL_SysWMinfo NativeWindow::sdl_window_info() const
    {
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        SDL_GetWindowWMInfo(m_sdl_window, &wmi);
        return wmi;
    }

    glm::vec2 NativeWindow::screen_size() const
    {
        glm::vec<2, int> size = { };
        SDL_GetWindowSize(m_sdl_window, &size.x, &size.y);
        return size;
    }

    SDL_Renderer *NativeWindow::create_renderer()
    {
        auto renderer = SDL_CreateRenderer(m_sdl_window, -1, 0);
        m_renderers.push_back(renderer);
        return renderer;
    }
}
