#pragma once

#include <memory>
#include <functional>

#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    using SurfaceProvider = std::function<std::shared_ptr<vk::raii::SurfaceKHR>(vk::raii::Instance&)>;
    std::shared_ptr<Instance> initialize(const GraphicsInfo&, const SurfaceProvider&);
}
