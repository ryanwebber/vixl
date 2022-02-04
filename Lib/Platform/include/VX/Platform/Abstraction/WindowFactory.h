#pragma once

#include <string_view>
#include <glm/glm.hpp>

#include <VX/Platform/NativeWindow.h>

namespace VX::Platform::Abstraction {

    struct WindowOptions {
        std::string_view name;
        glm::vec<2, int> size;
    };

    class WindowFactory {
    protected:
        WindowFactory() = default;

    public:
        ~WindowFactory() = default;

        [[nodiscard]] virtual std::shared_ptr<VX::Platform::NativeWindow> create_with_options(const WindowOptions&) const = 0;
    };
}
