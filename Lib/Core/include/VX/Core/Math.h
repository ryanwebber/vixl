#pragma once

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

namespace VX::Core::Math {
    template<typename T>
    T lerp(T a, T b, float t) {
        return a * (1.f - t) + b * t;
    }

    template<typename T>
    T lerp_unclamped(T a, T b, float t) {
        return std::clamp(a, b, Lerp(a, b, t));
    }

    template<typename T>
    float inverse_lerp(T a, T b, T value) {
        if (a != b)
            return (value - a) / (b - a);
        else
            return a;
    }

    template<typename T>
    float inverse_lerped_clamped(T a, T b, T value) {
        return std::clamp(0.0f, 1.0f, InverseLerp(a, b, value));
    }
}