#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <VX/Noncopyable.h>

#include <VX/Core/Size.h>

namespace VX::Core {
    struct ViewProjection {
        glm::mat4x4 view_matrix { glm::mat4x4(1) };
        glm::mat4x4 projection_matrix { glm::mat4x4(1) };
    };

    class RenderContext final {
    };
}
