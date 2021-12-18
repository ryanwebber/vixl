#pragma once

#include <vector>
#include <memory>

#include <Common/Noncopyable.h>
#include <Core/UpdateSystem.h>

namespace Core {
    class Scene final {
        VX_MAKE_NONMOVABLE(Scene);
        VX_MAKE_NONCOPYABLE(Scene);

    public:
        Scene() = default;
        ~Scene() = default;

        void Update(UpdateSystem &system);
    };
}
