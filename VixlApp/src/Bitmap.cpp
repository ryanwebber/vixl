#include <pixman/pixman.h>

#include <App/Bitmap.h>
#include <App/Logger.h>

void dealloc_image(pixman_image_t* img) {
#ifdef DEBUG_MEMORY
    Logger::Core->debug("Deallocating unmanaged buffer data ptr={}", static_cast<void*>(img));
#endif
    pixman_image_unref(img);
}

PixmanContainer CreateContainer(const SizeInt &size) {
    auto [width,height] = size.dimensions;
    auto format = PIXMAN_a8r8g8b8;
    auto image = pixman_image_create_bits(format, width, height,nullptr, 0);
    auto data = pixman_image_get_data(image);
    auto length = width * height * PIXMAN_FORMAT_BPP(format);

    std::shared_ptr<pixman_image_t> img_ptr(image, dealloc_image);
    UnmanagedBuffer<uint32_t> buffer(img_ptr, data, length);

    return {
        .dimensions = size,
        .data = std::move(buffer),
        .image = std::move(img_ptr),
    };
}

Bitmap::Bitmap(SizeInt size)
    : m_Container(CreateContainer(size))
{
}
