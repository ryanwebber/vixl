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

    virtual ~RenderLayer() = default;
};
