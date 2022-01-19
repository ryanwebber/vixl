#include <VX/Core/RenderStack.h>

namespace VX::Core {

    void RenderStack::update() const {
        for (auto &&layer: m_layers) {
            layer->on_update();
        }
    }

    void RenderStack::render() const {
        for (auto &&layer: m_layers)
            layer->on_will_render();

        for (auto &&layer: m_layers)
            layer->on_render();

        for (auto &&layer: m_layers)
            layer->on_did_render();
    }

    void RenderStack::add_layer(std::shared_ptr<RenderLayer> layer) {
        layer->on_initialize();
        m_layers.push_back(std::move(layer));
    }

    void RenderStack::destroy() const {
        for (auto &&layer: m_layers)
            layer->on_destroy();
    }
}
