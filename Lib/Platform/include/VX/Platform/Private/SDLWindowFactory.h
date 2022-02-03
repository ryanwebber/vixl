#pragma once

#include <memory>

#include <VX/Platform/Abstraction/WindowFactory.h>

namespace VX::Platform::Private {

    template <uint32_t FLAGS>
    class SDLWindowFactory: public Abstraction::WindowFactory {
    private:
        SDLWindowFactory() = default;

    public:
        std::shared_ptr<VX::Platform::NativeWindow> create_with_options(const Abstraction::WindowOptions &options) const override {
            SDL_Init(SDL_INIT_EVERYTHING);

            SDL_Window* window = SDL_CreateWindow(
                    options.name.data(),
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    options.size.width,
                    options.size.height,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | FLAGS);

            return std::make_shared<NativeWindow>(window);
        }

        static const Abstraction::WindowFactory& instance() {
            static SDLWindowFactory<FLAGS> s_instance;
            return s_instance;
        }
    };
}
