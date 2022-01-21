#pragma once

#include <span>
#include <bgfx/bgfx.h>

#include <VX/Expected.h>
#include <VX/Copyable.h>
#include <VX/Core/Types.h>

namespace VX::Core {
    class Texture final {
        VX_DEFAULT_MOVABLE(Texture);
        VX_DEFAULT_COPYABLE(Texture);
    private:
        std::shared_ptr<TextureHandle> m_handle;
        bgfx::TextureInfo m_texture_info;

    public:
        Texture(std::shared_ptr<TextureHandle> handle, bgfx::TextureInfo info)
            : m_handle(std::move(handle))
            , m_texture_info(info)
            {
            }

        ~Texture() = default;

        [[nodiscard]] const bgfx::TextureInfo& texture_info() const { return m_texture_info; }
        [[nodiscard]] const std::shared_ptr<TextureHandle>& texture_handle() const { return m_handle; }

        static VX::Expected<Texture> create(std::span<const std::byte> data, uint64_t flags);
    };
}
