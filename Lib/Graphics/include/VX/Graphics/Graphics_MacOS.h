#pragma once

#include <memory>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Graphics.h>

namespace VX::Graphics::MacOS {

    class CAMetalLayer;

    struct PlatformData {
        const CAMetalLayer *metal_layer { nullptr };
    };

    std::shared_ptr<Instance> initialize(const GraphicsInfo&, const PlatformData&);
}
