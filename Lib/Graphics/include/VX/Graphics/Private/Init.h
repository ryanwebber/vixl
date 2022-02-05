#pragma once

#include <memory>
#include <functional>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Private/Vulkan.h>
#include <VX/Graphics/Private/PlatformDelegate.h>

namespace VX::Graphics::Private {
    std::shared_ptr<Instance> initialize(const GraphicsInfo&, const PlatformDelegate&);
}
