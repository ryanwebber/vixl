#pragma once

#include <Core/RenderCommand.h>

namespace Core {
    class RenderPass final {
    public:
        void Submit(const RenderCommand&);
    };
}
