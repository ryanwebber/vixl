#pragma once

#include <memory>

#include <entt/entity/registry.hpp>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Core/RenderBuffer.h>
#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderPass.h>

namespace VX::Core {
    class RenderSystem {
    VX_MAKE_NONMOVABLE(RenderSystem);
    VX_MAKE_NONCOPYABLE(RenderSystem);

    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;

        virtual void render(const RenderContext&, RenderBuffer&, const entt::registry&) = 0;
    };
}
