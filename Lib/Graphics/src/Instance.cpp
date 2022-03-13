#include <VX/Graphics/Instance.h>

namespace VX::Graphics {
    Instance::Instance(std::shared_ptr<InstanceImpl> impl)
            : m_impl(std::move(impl))
    {}

    Instance::Instance(Instance&&) noexcept = default;
    Instance& Instance::operator=(Instance&&) noexcept = default;

    Instance::Instance(const Instance&) noexcept = default;
    Instance& Instance::operator=(const Instance&) noexcept = default;

    Instance::~Instance() = default;

    const InstanceImpl* Instance::operator->() const {
        return m_impl.get();
    }

    InstanceImpl* Instance::operator->() {
        return m_impl.get();
    }

    const InstanceImpl &Instance::operator*() const {
        return *m_impl;
    }

    InstanceImpl &Instance::operator*() {
        return *m_impl;
    }

    // Instance Impl

    VX::Expected<void> InstanceImpl::begin_render_pass(const RenderContextHandle&, const RenderTargetHandle&, const CommandBufferHandle&) {
        return VX::make_unexpected("Unimplemented");
    }

    VX::Expected<void> InstanceImpl::end_render_pass() {
        return VX::make_unexpected("Unimplemented");
    }

    VX::Expected<SwapState> InstanceImpl::begin_frame() {
        return m_swapchain.try_acquire_next_swap_state();
    }

    VX::Expected<void> InstanceImpl::end_frame(const SwapState &swap_state) {
        return m_swapchain.try_present_and_swap(swap_state);
    }

    void InstanceImpl::bind(const GraphicsPipelineHandle&) {

    }

    void InstanceImpl::draw() {

    }
}
