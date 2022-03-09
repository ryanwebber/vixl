#include <VX/Graphics/Graphics.h>
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

    // ---------------------

    template<HandleType T>
    Handle<T>::Handle(HandleIdentifier identifier, std::shared_ptr<Instance> instance)
            : m_identifier(identifier)
            , m_instance(std::move(instance))
    {
    }

    template<HandleType T>
    Handle<T>::Handle(Handle<T> &&) noexcept = default;

    template<HandleType T>
    Handle<T> &Handle<T>::operator=(Handle<T> &&) noexcept = default;

    template<HandleType T>
    Handle<T>::~Handle()
    {
        (void)this;
        (*m_instance)->template destroy_handle(*this);
    }
}
