#include <string>

#include <VX/Core/Texture.h>

namespace VX::Core {
    VX::Expected<Texture> Texture::create(std::span<const std::byte> data, uint64_t flags) {
#warning "Not implemented"
        return Texture(nullptr);
    }
}
