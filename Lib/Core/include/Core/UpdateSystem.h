#pragma once

#include <Common/Noncopyable.h>

namespace Core {
    class UpdateSystem {
        VX_MAKE_NONMOVABLE(UpdateSystem);
        VX_MAKE_NONCOPYABLE(UpdateSystem);

    public:
        UpdateSystem() = default;
        virtual ~UpdateSystem() = default;

        virtual void Update() = 0;
    };
}
