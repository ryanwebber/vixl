#pragma once

#include <span>
#include <bgfx/bgfx.h>

#include <VX/Expected.h>
#include <VX/Core/Types.h>

namespace VX::Core {
    class Texture final {
    private:
        std::shared_ptr<TextureHandle> m_Handle;
        bgfx::TextureInfo m_TextureInfo;

    public:
        Texture(std::shared_ptr<TextureHandle> handle, bgfx::TextureInfo info)
            : m_Handle(std::move(handle))
            , m_TextureInfo(info)
            {
            }

        ~Texture() = default;

        // Copy
        Texture(const Texture&) noexcept = default;
        Texture& operator=(const Texture&) noexcept = default;

        // Move
        Texture(Texture&& other) noexcept
            : m_Handle(std::move(other.m_Handle))
            , m_TextureInfo(other.m_TextureInfo)
            {
            }

        Texture& operator=(Texture&&) = default;

        [[nodiscard]] const bgfx::TextureInfo& GetInfo() const { return m_TextureInfo; }

        std::shared_ptr<TextureHandle> GetHandleRef() { return m_Handle; }
        [[nodiscard]] const TextureHandle& GetHandle() const { return *m_Handle; }

        static VX::Expected<Texture> Create(std::span<const uint8_t>, uint64_t flags);
    };
}
