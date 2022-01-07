#pragma once

#include <glm/glm.hpp>

namespace Core {
    struct SceneCamera {
        glm::mat4x4 projection;

        SceneCamera()
            : projection(glm::mat4x4(1))
        { }
    };
}
