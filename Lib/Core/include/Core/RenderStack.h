#pragma once

#include <Common/Noncopyable.h>
#include <Core/RenderLayer.h>

#include <memory>
#include <vector>

namespace Core {

    class RenderStack final {
        VX_MAKE_NONCOPYABLE(RenderStack);
        VX_MAKE_NONMOVABLE(RenderStack);

    private:
        std::vector<std::shared_ptr<RenderLayer>> m_Layers;

    public:
        RenderStack() = default;
        ~RenderStack() = default;

        void Update() const;
        void Render() const;

        void AddLayer(std::shared_ptr<RenderLayer> layer);
        void Destroy() const;
    };
}