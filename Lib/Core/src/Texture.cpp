#include <string>

#include <bimg/bimg.h>
#include <bx/error.h>

#include <VX/Core/Texture.h>

static void imageReleaseCb(void*, void* _userData)
{
    auto imageContainer = static_cast<bimg::ImageContainer*>(_userData);
    bimg::imageFree(imageContainer);
}

namespace VX::Core {
    VX::Expected<Texture> Texture::Create(std::span<const uint8_t> data, uint64_t flags) {
        bx::Error err;
        auto imageContainer = new bimg::ImageContainer();
        if (!bimg::imageParse(*imageContainer, data.data(), data.size(), &err)) {
            std::string err_msg(err.getMessage().getPtr(), err.getMessage().getLength());
            auto our_err = VX::Error(err_msg).with_context_format("Unable to create texture");
            return VX::Unexpected(std::move(our_err));
        }

        const bgfx::Memory* mem = bgfx::makeRef(imageContainer->m_data,
                                                imageContainer->m_size,
                                                imageReleaseCb,
                                                imageContainer);

        if (!bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), flags)) {
            imageReleaseCb(nullptr, imageContainer);
            return VX::MakeUnexpected<Texture>("Invalid texture data");
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

        return Texture(std::move(std::make_shared<TextureHandle>(handle)), texture_info);
    }
}
