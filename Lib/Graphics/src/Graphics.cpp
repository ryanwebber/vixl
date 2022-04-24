#include <VX/Graphics/Graphics.h>

namespace VX::Graphics {
    RenderTargetHandle SwapchainInfo::render_target() const {
        return RenderTargetHandle(0);
    }
}
