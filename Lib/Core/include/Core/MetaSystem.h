#pragma once

#include <memory>
#include <future>

namespace Core {
    class Scene;
    class MetaSystem {
    public:
        ~MetaSystem() = default;
        virtual void Configure(std::weak_ptr<Scene>) = 0;
    };
}
