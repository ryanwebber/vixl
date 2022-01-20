#include <string>

#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <bx/error.h>

#include <VX/Core/Logger.h>
#include <VX/Core/Texture.h>

static void image_release_cb(void*, void* _userData)
{
    auto imageContainer = static_cast<bimg::ImageContainer*>(_userData);
    bimg::imageFree(imageContainer);
}

namespace VX::Core {

    static bx::DefaultAllocator s_texture_allocator;

    VX::Expected<Texture> Texture::create(std::span<const std::byte> data, uint64_t flags) {
        bx::Error err;
        auto imageContainer = bimg::imageParse(&s_texture_allocator, data.data(), data.size(), bimg::TextureFormat::Count, &err);

        if (!err.isOk()) {
            std::string err_msg(err.getMessage().getPtr(), err.getMessage().getLength());
            auto our_err = VX::Error(err_msg).with_context_format("Unable to create texture");
            return VX::Unexpected(std::move(our_err));
        }

        const bgfx::Memory* mem = bgfx::makeRef(imageContainer->m_data,
                                                imageContainer->m_size,
                                                image_release_cb,
                                                imageContainer);

        if (!bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), flags)) {
            image_release_cb(nullptr, imageContainer);
            return VX::make_unexpected<Texture>("Invalid texture data");
        }

        auto handle = bgfx::createTexture2D(uint16_t(imageContainer->m_width),
                                            uint16_t(imageContainer->m_height),
                                            1 < imageContainer->m_numMips,
                                            imageContainer->m_numLayers,
                                            bgfx::TextureFormat::Enum(imageContainer->m_format),
                                            flags,
                                            mem);

        bgfx::TextureInfo texture_info = {};
        bgfx::calcTextureSize(texture_info,
                              uint16_t(imageContainer->m_width),
                              uint16_t(imageContainer->m_height),
                              uint16_t(imageContainer->m_depth),
                              imageContainer->m_cubeMap,
                              1 < imageContainer->m_numMips,
                              imageContainer->m_numLayers,
                              bgfx::TextureFormat::Enum(imageContainer->m_format));

        Logger::Core->debug("Loaded texture: size={}x{} format={}", texture_info.width, texture_info.height, texture_info.format);

        return Texture(std::move(std::make_shared<TextureHandle>(handle)), texture_info);
    }
}
