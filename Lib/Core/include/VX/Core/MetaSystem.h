#pragma once

#include <memory>
#include <future>

namespace VX::Core {
    class Scene;
    class MetaSystem {
    public:
        ~MetaSystem() = default;
        virtual void configure(std::weak_ptr<Scene>) = 0;
    };
}
