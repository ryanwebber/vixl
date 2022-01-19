#pragma once

#include <VX/Core/RenderCommand.h>

namespace VX::Core {
    class RenderPass final {
    public:
        void Submit(const RenderCommand&);
    };
}
