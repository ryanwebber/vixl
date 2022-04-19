#pragma once

#include <functional>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>

namespace VX::Core {
    struct RenderTarget {

        class AllocationRequest {
            VX_MAKE_NONCOPYABLE(AllocationRequest);
            VX_DEFAULT_MOVABLE(AllocationRequest);
        private:
            std::function<RenderTarget(Graphics::Instance&)> m_initializer;
            explicit AllocationRequest(std::function<RenderTarget(Graphics::Instance&)> initializer)
                : m_initializer(std::move(initializer))
            {};

        public:
            ~AllocationRequest() = default;

            RenderTarget construct(Graphics::Instance& instance) const {
                return m_initializer(instance);
            }

            static AllocationRequest with_texture();
            static AllocationRequest with_swapchain();
        };

        Graphics::RenderTargetHandle handle;
    };
}
