#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace VX::Graphics {
    struct GraphicsInfo {
        std::vector<const char*> required_extensions;
        glm::vec<2, int> framebuffer_dimensions;
    };
}
