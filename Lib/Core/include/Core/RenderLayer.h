#pragma once

namespace Core {
    class RenderLayer {
    private:
    public:
        RenderLayer() = default;
        virtual ~RenderLayer() = default;

        virtual void OnInitialize() {};
        virtual void OnDestroy() {};
        virtual void OnUpdate() {};
        virtual void OnWillRender() {};
        virtual void OnRender() = 0;
        virtual void OnDidRender() {};
    };
}
