#pragma once

#include <glm/glm.hpp>

template<typename T>
union SizeT {
    glm::vec<2, T, glm::defaultp> vec;
    struct {
        T width;
        T height;
    };
    struct {
        T width;
        T height;
    } dimensions;

    template<typename D>
    SizeT<D> cast() const;
};

typedef SizeT<float> Size;
typedef SizeT<int> SizeInt;

template<typename T>
template<typename D>
SizeT<D> SizeT<T>::cast() const {
    return {
        .width = static_cast<D>(width),
        .height = static_cast<D>(height),
    };
}
