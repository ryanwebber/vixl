#include <VX/Graphics/Instance.h>

namespace VX::Graphics {
    template<>
    void InstanceImpl::destroy_handle(const Handle<HandleType::GraphicsPipeline> &handle) {
        m_graphics_pipelines.erase(handle.identifier());
    }
}
