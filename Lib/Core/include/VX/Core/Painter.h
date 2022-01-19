#pragma once

#include <functional>

#include <glm/glm.hpp>
#include <utility>
#include <pixman/pixman.h>
#include <VX/Noncopyable.h>

#include <VX/Core/Bitmap.h>

namespace VX::Core {
    class Painter final {
    VX_MAKE_NONCOPYABLE(Painter);

    VX_MAKE_NONMOVABLE(Painter);

    private:
        Bitmap m_Bitmap;
    public:
        explicit Painter(Bitmap &bitmap)
                : m_Bitmap(bitmap) {};

        ~Painter() = default;

        void Draw(const std::function<void(pixman_image_t *)> &draw_fn) {
            auto image = m_Bitmap.GetImage();
            draw_fn(image);
        }
    };
}