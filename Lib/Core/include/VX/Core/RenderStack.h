#pragma once

#include <memory>
#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Core/RenderLayer.h>

namespace VX::Core {
    class RenderStack final {
        VX_MAKE_NONCOPYABLE(RenderStack);
        VX_MAKE_NONMOVABLE(RenderStack);

    private:
        std::vector<std::shared_ptr<RenderLayer>> m_layers;

    public:
        RenderStack() = default;
        ~RenderStack() = default;

        void update() const;
        void render() const;

        void add_layer(std::shared_ptr<RenderLayer> layer);
        void destroy() const;
    };
}
