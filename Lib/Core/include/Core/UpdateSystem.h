#pragma once

#include <Common/Noncopyable.h>

#include <entt/entity/registry.hpp>

namespace Core {
    class UpdateSystem {
        VX_MAKE_NONMOVABLE(UpdateSystem);
        VX_MAKE_NONCOPYABLE(UpdateSystem);

    public:
        UpdateSystem() = default;
        virtual ~UpdateSystem() = default;

        virtual void Update(entt::registry& entities) = 0;
    };
}
