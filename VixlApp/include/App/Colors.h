#pragma once

#include <glm/glm.hpp>

union Color {
    glm::vec4 vec;
    struct {
        float r;
        float g;
        float b;
        float a;
    };
    struct {
        float r;
        float g;
        float b;
        float a;
    } rgba;
};

namespace Colors {
    const Color transparent { };
    const Color workspace_background = { .r = 0.1, .g = 0.1, .b = 0.1, .a = 1.0 };
}
