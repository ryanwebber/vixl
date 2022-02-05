#pragma once

#include <string_view>

#include <VX/Graphics/Instance.h>
#include <VX/Platform/NativeWindow.h>

namespace VX::Platform::Abstraction {

    class GraphicsInitializer {
    protected:
        GraphicsInitializer() = default;

    public:
        ~GraphicsInitializer() = default;

        [[nodiscard]] virtual std::shared_ptr<Graphics::Instance> initialize_with_window(NativeWindow&) const = 0;
    };
}
