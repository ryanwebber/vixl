#include "SDL.h"

#include <VX/Core/Input.h>

namespace VX::Core {
    ControlState Input::process_events() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return ControlState::Terminate;
                default:
                    break;
            }
        }

        return ControlState::None;
    }
}
