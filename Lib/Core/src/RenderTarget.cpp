#include <VX/Core/Assert.h>
#include <VX/Core/RenderTarget.h>

namespace VX::Core {

    RenderTarget::AllocationRequest RenderTarget::AllocationRequest::with_texture()
    {
        VX_CORE_ASSERT_NOT_REACHED();
    }

    RenderTarget::AllocationRequest RenderTarget::AllocationRequest::with_swapchain()
    {
        VX_CORE_ASSERT_NOT_REACHED();
    }
}
