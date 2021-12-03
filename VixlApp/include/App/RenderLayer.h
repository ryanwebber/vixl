#pragma once

class RenderLayer {
private:
public:
    virtual void OnInitialize() { };
    virtual void OnDestroy() { };

    virtual void OnUpdate() { };

    virtual void OnWillRender() { };
    virtual void OnRender() = 0;
    virtual void OnDidRender() { };

    virtual void OnWindowResize(int width, int height) { };

    virtual ~RenderLayer() = default;
};
