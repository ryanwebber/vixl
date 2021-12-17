#pragma once

#include <Core/RenderPass.h>

namespace Core {
    class RenderCommand {
    public:
        RenderCommand() = default;
        virtual ~RenderCommand() = default;

        virtual void Render(RenderPass &pass) const = 0;
    };
}
