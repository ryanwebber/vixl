#pragma once

#include <string_view>

#include <VX/Graphics/Graphics.h>
#include <VX/Platform/NativeWindow.h>

namespace VX::Platform::Abstraction {

    class GraphicsInitializer {
    protected:
        GraphicsInitializer() = default;

    public:
        ~GraphicsInitializer() = default;

        [[nodiscard]] virtual Graphics::Instance initialize_with_window(NativeWindow&) const = 0;
    };
}
