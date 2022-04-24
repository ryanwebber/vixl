#pragma once

#include <functional>
#include <variant>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>

namespace VX::Core {
    class RenderTarget final {
        VX_DEFAULT_MOVABLE(RenderTarget);
        VX_DEFAULT_COPYABLE(RenderTarget);
    private:
        Graphics::SharedHandle<Graphics::HandleType::RenderTarget> m_handle;
    public:
        class BackingStore {
            VX_DEFAULT_MOVABLE(BackingStore);
            VX_DEFAULT_COPYABLE(BackingStore);
        public:
            struct BufferBacked {
            };

            struct SwapchainBacked {
            };

            using Descriptor = std::variant<BufferBacked, SwapchainBacked>;

        private:
            Descriptor m_descriptor;
            explicit BackingStore(const Descriptor &descriptor)
                : m_descriptor(descriptor)
            {};

        public:
            ~BackingStore() = default;

            [[nodiscard]] const Descriptor& descriptor() const { return m_descriptor; }

            static BackingStore graphics_buffer() {
                return BackingStore(BufferBacked());
            }

            static BackingStore swapchain_buffer() {
                return BackingStore(SwapchainBacked());
            }
        };

        explicit RenderTarget(Graphics::SharedHandle<Graphics::HandleType::RenderTarget> handle)
            : m_handle(std::move(handle))
        {};

        ~RenderTarget() = default;

        [[nodiscard]] const Graphics::RenderTargetHandle &handle() const { return *m_handle; }
    };
}
