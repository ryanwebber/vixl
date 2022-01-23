#pragma once

#include <memory>
#include <VX/Core/Material.h>

namespace VX::Core::Component {
    struct SpriteComponent final {
        std::shared_ptr<Material> material;
    };
}
