#pragma once

#include <memory>
#include <functional>

#include <VX/Expected.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/PlatformDelegate.h>

namespace VX::Graphics {
    VX::Expected<Instance> init(const GraphicsInfo&, const PlatformDelegate&);
}
