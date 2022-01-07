#pragma once

#include <memory>

#include <Core/RenderTarget.h>
#include <Core/SceneCamera.h>

namespace Core {
    class RenderSystem {
    VX_MAKE_NONMOVABLE(RenderSystem);
    VX_MAKE_NONCOPYABLE(RenderSystem);

    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;

        virtual void Render(RenderTarget&, SceneCamera const &camera) = 0;
    };
}
