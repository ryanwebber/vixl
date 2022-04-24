#pragma once

#include <memory>
#include <filesystem>

#include <VX/Core/Async.h>
#include <VX/Core/ResourceManager.h>
#include <VX/Core/Texture.h>
#include "Material.h"

namespace VX::Core {
    class RenderBuiltins final {
    private:
    public:
        explicit RenderBuiltins(AssetBundle&);
        ~RenderBuiltins() = default;
    };
}
