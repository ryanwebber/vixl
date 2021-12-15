#pragma once

#include <memory>

#include <pixman/pixman.h>
#include <Common/Copyable.h>
#include <Common/UnmanagedBuffer.h>

#include <Core/Size.h>

namespace Core {

    class Painter;

    struct PixmanContainer final {
        SizeInt dimensions;
        Common::UnmanagedBuffer<uint32_t> data;
        std::shared_ptr<pixman_image_t> image;
    };

    class Bitmap final {
        VX_DEFAULT_COPYABLE(Bitmap);
        VX_DEFAULT_MOVABLE(Bitmap);

    private:
        PixmanContainer m_Container;

        [[nodiscard]] pixman_image_t *GetImage() const { return m_Container.image.get(); }

    public:
        explicit Bitmap(SizeInt size);

        ~Bitmap() = default;

        [[nodiscard]] SizeInt GetSize() const { return m_Container.dimensions; }

        [[nodiscard]] Common::UnmanagedBuffer<uint32_t> &GetBuffer() { return m_Container.data; }

        [[nodiscard]] const Common::UnmanagedBuffer<uint32_t> &GetBuffer() const { return m_Container.data; }

        friend class Painter;
    };
}
