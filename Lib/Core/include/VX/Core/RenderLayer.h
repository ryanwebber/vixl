#pragma once

namespace VX::Core {
    class RenderLayer {
    private:
    public:
        RenderLayer() = default;
        virtual ~RenderLayer() = default;

        virtual void on_initialize() {};
        virtual void on_destroy() {};

        virtual void on_update() {};

        virtual void on_will_render() {};
        virtual void on_render() = 0;
        virtual void on_did_render() {};
    };
}
