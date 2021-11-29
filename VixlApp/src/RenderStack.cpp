#include <App/RenderStack.h>

void RenderStack::Update() const {
    for (auto &&layer : m_Layers) {
        layer->OnUpdate();
    }
}

void RenderStack::Render() const {
    for (auto &&layer : m_Layers) {
        layer->OnRender();
    }
}

void RenderStack::AddLayer(std::unique_ptr<RenderLayer> layer) {
    layer->OnInitialize();
    m_Layers.push_back(std::move(layer));
}

void RenderStack::Destroy() const {
    for (auto &&layer : m_Layers) {
        layer->OnDestroy();
    }
}
