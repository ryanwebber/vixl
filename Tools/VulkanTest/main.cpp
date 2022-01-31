#include <VX/Entry/Main.h>
#include <VX/Graphics/Instance.h>
#include <VX/Core/NativeWindow.h>
#include <VX/Core/Input.h>

int vixl_main(const VX::Entry::Context &ctx) {

    auto window = VX::Core::NativeWindow::create_with_size({ .width = 800, .height = 600 }).value();
    auto graphics = VX::Graphics::init({
        .platform_data = {
            .native_window_handle = nullptr,
            .required_extensions = std::move(window.graphics_extensions())
        }
    });

    return 0;
}
