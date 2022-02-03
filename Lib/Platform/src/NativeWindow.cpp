#include "SDL.h"
#include "SDL_syswm.h"

#include <VX/Platform/NativeWindow.h>

namespace VX::Platform {
    NativeWindow::~NativeWindow()
    {
        SDL_DestroyWindow(m_sdl_window);
    }

    SDL_SysWMinfo NativeWindow::sdl_window_info() const
    {
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        SDL_GetWindowWMInfo(m_sdl_window, &wmi);
        return wmi;
    }

    Viewport NativeWindow::viewport() const
    {
        Viewport viewport;
        SDL_GetWindowSize(m_sdl_window, &viewport.width, &viewport.height);
        return viewport;
    }
}
