#pragma once

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/CommandBuffer.h>

namespace VX::Graphics {

    namespace Private {
        class FramebufferImpl;
    }

    class Framebuffer final : public APIObject<Private::FramebufferImpl> {
    public:
        using APIObject<Private::FramebufferImpl>::APIObject;
    };
}
