#pragma once

#include <VX/Noncopyable.h>

#include <entt/entity/registry.hpp>

namespace VX::Core {
    class UpdateSystem {
        VX_MAKE_NONMOVABLE(UpdateSystem);
        VX_MAKE_NONCOPYABLE(UpdateSystem);

    public:
        UpdateSystem() = default;
        virtual ~UpdateSystem() = default;

        virtual void update(entt::registry& entities) = 0;
    };
}
