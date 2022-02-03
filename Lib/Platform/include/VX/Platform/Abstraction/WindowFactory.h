#pragma once

#include <string_view>

#include <VX/Platform/NativeWindow.h>

namespace VX::Platform::Abstraction {

    struct WindowOptions {
        std::string_view name;
        struct {
            int width = 800;
            int height = 600;
        } size;
    };

    class WindowFactory {
    protected:
        WindowFactory() = default;

    public:
        ~WindowFactory() = default;

        virtual std::shared_ptr<VX::Platform::NativeWindow> create_with_options(const WindowOptions&) const = 0;
    };
}
