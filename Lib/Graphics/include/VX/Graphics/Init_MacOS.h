#pragma once

#include <memory>

#include <VX/Graphics/Graphics.h>

namespace VX::Graphics::MacOS {

    class CAMetalLayer;

    struct PlatformData {
        const CAMetalLayer *metal_layer { nullptr };
    };

    VX::Expected<Instance> init(const GraphicsInfo &info, const PlatformData&);
}
