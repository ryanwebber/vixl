#include "SDL.h"

#include <VX/Core/Input.h>

namespace VX::Core {
    Input::Input(std::shared_ptr<NativeWindow> window, Executor &executor)
        : m_native_window(std::move(window))
        , m_on_window_resize_event(executor)
    {
    }

    ControlState Input::process_events() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return ControlState::Terminate;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            m_on_window_resize_event.publisher().publish({{
                                .width = event.window.data1,
                                .height = event.window.data2,
                            }});
                        break;
                    }
                    break;
                default:
                    break;
            }
        }

        return ControlState::None;
    }
}
