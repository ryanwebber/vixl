#pragma once

#include <VX/Graphics/APIObject.h>

namespace VX::Graphics {

    namespace Private {
        class CommandBufferImpl;
    }

    class CommandBuffer final : public APIObject<Private::CommandBufferImpl> {
    public:
        using APIObject<Private::CommandBufferImpl>::APIObject;
    };
}
