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
}
