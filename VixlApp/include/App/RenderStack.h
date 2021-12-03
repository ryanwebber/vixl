#pragma once

#include <Common/Noncopyable.h>
#include <App/RenderLayer.h>

#include <memory>
#include <vector>

class RenderStack final {
    VX_MAKE_NONCOPYABLE(RenderStack);
    VX_MAKE_NONMOVABLE(RenderStack);

private:
    std::vector<std::unique_ptr<RenderLayer>> m_Layers;

public:
    RenderStack() = default;
    ~RenderStack() = default;

    void Update() const;
    void Render() const;

    void OnWindowResize(int width, int height) const;

    void AddLayer(std::unique_ptr<RenderLayer> layer);
    void Destroy() const;
};
