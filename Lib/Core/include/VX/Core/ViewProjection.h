#pragma once

#include <glm/glm.hpp>

namespace VX::Core {
    struct ViewProjection {
        glm::mat4x4 view_matrix { glm::mat4x4(1) };
        glm::mat4x4 projection_matrix { glm::mat4x4(1) };
    };
}
