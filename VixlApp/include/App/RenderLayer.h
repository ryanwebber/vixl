#pragma once

class RenderLayer {
private:
public:
    virtual void OnInitialize() { };
    virtual void OnDestroy() { };

    virtual void OnUpdate() { };
    virtual void OnRender() const = 0;

    virtual ~RenderLayer() { };
};
