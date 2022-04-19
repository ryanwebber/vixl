#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {
    class RenderTarget final {
        VX_MAKE_NONCOPYABLE(RenderTarget);
        VX_DEFAULT_MOVABLE(RenderTarget);
    private:
        vk::raii::ImageView m_image_view;
        vk::raii::Framebuffer m_framebuffer;
        vk::Extent2D m_extents;
    public:
        RenderTarget(vk::raii::ImageView image_view, vk::raii::Framebuffer framebuffer, vk::Extent2D extents)
                : m_image_view(std::move(image_view))
                , m_framebuffer(std::move(framebuffer))
                , m_extents(extents)
        {}

        ~RenderTarget() = default;

        vk::raii::Framebuffer &framebuffer() { return m_framebuffer; }
        [[nodiscard]] const vk::raii::Framebuffer &framebuffer() const { return m_framebuffer; }

        [[nodiscard]] vk::Extent2D extents() const { return m_extents; }
    };
}
