#pragma once

#include <functional>

#include "Expected.h"

namespace VX {
    template <class R, class E>
    VX::Expected<R> try_catch(const std::function<R()> &f) {
        try {
            return f();
        } catch (const E &err) {
            return VX::make_unexpected("Error: {}", err.what());
        }
    }

    template <class E>
    VX::Expected<void> try_catch(const std::function<void()> &f) {
        try {
            f();
            return { };
        } catch (const E &err) {
            return VX::make_unexpected("Error: {}", err.what());
        }
    }
}
