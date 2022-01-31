#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_vulkan.h"

#include <VX/Core/NativeWindow.h>
#include <VX/Core/Platform/Platform.h>

namespace VX::Core {

    SizeInt NativeWindow::size() const {
        SizeInt actual_window_size;
        SDL_GetWindowSize(m_window, &actual_window_size.width, &actual_window_size.height);
        return actual_window_size;
    }

    void NativeWindow::destroy() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void NativeWindow::clear() {
//        SDL_RenderClear(m_renderer);
    }

    void NativeWindow::swap_buffers() {
//        SDL_RenderPresent(m_renderer);
    }

    VX::Expected<NativeWindow> NativeWindow::create_with_size(SizeInt window_size) {

        SDL_Init(SDL_INIT_EVERYTHING);

        // Quick and dirty test to see if we can support the current device with Vulkan
        if (SDL_Vulkan_LoadLibrary(nullptr) != 0) {
            return VX::make_unexpected<NativeWindow>("No vulkan support found.");
        }

        SDL_Window* window = SDL_CreateWindow("Vixl " VX_VERSION,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              SDL_WINDOWPOS_UNDEFINED,
                                              window_size.width,
                                              window_size.height,
                                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(window, &wmi)) {
            return VX::make_unexpected<NativeWindow>("Unable to create window");
        }

        bgfx::PlatformData pd;
        pd.context = nullptr;
        pd.backBuffer = nullptr;
        pd.backBufferDS = nullptr;

        Platform::Current::initialize_platform_data(wmi, pd);

        auto renderer = SDL_CreateRenderer(window, 0, {});

        return NativeWindow(window, renderer, pd);
    }

    std::vector<const char *> NativeWindow::graphics_extensions() const
    {
        unsigned int extensionCount = 0;
        SDL_Vulkan_GetInstanceExtensions(nullptr, &extensionCount, nullptr);
        std::vector<const char*> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(nullptr, &extensionCount, extensionNames.data());
        return extensionNames;
    }
}
