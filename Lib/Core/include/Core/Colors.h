#pragma once

#include <algorithm>

#include <glm/glm.hpp>
#include <pixman/pixman.h>

namespace Core {

    template<typename T>
    union ColorT {
        glm::vec<4, T, glm::defaultp> vec;
        struct {
            T r;
            T g;
            T b;
            T a;
        };
        struct {
            T r;
            T g;
            T b;
            T a;
        } rgba;

        template<typename D>
        ColorT<D> cast() const;

        template<typename D>
        D reinterpret() const;
    };

    typedef ColorT<float> Color;

    template<>
    template<typename D>
    ColorT<D> ColorT<float>::cast() const {
        float white = std::numeric_limits<D>::max();
        return {
                .r = static_cast<D>(std::clamp(r, 0.0f, 1.0f) * white),
                .g = static_cast<D>(std::clamp(g, 0.0f, 1.0f) * white),
                .b = static_cast<D>(std::clamp(b, 0.0f, 1.0f) * white),
                .a = static_cast<D>(std::clamp(a, 0.0f, 1.0f) * white),
        };
    }

    template<typename T>
    template<typename D>
    D ColorT<T>::reinterpret() const {
        return {r, g, b, a};
    }

    namespace Colors {
        const Color transparent{};
        const Color workspace_background = {.r = 1.0, .g = 0.0, .b = 1.0, .a = 1.0};
    }
}
