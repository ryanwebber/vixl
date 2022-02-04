#pragma once

#include <memory>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Graphics.h>

namespace VX::Graphics::MacOS {
    struct PlatformData {
        void* native_window_handle;
    };

    std::shared_ptr<Instance> initialize(const GraphicsInfo&, const PlatformData&);
}
