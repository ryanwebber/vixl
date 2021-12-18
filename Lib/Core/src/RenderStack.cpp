#include <Core/RenderStack.h>

namespace Core {

    void RenderStack::Update() const {
        for (auto &&layer: m_Layers) {
            layer->OnUpdate();
        }
    }

    void RenderStack::Render() const {
        for (auto &&layer: m_Layers)
            layer->OnWillRender();

        for (auto &&layer: m_Layers)
            layer->OnRender();

        for (auto &&layer: m_Layers)
            layer->OnDidRender();
    }

    void RenderStack::AddLayer(std::shared_ptr <RenderLayer> layer) {
        layer->OnInitialize();
        m_Layers.push_back(std::move(layer));
    }

    void RenderStack::Destroy() const {
        for (auto &&layer: m_Layers)
            layer->OnDestroy();
    }
}
