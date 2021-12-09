#pragma once

#include <glm/glm.hpp>

union Size {
    glm::vec2 vec;
    struct {
        float width;
        float height;
    };
    struct {
        float width;
        float height;
    } dimensions;
};
