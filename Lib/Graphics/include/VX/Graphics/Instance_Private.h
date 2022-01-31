#pragma once

#include <memory>

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace VX::Graphics::Private {
    class InstanceData {
    private:
        vk::raii::Context m_context;
        vk::raii::Instance m_instance;

    public:
        InstanceData(vk::raii::Context context, vk::raii::Instance instance)
                : m_context(std::move(context))
                , m_instance(std::move(instance))
        {}

        ~InstanceData() = default;

        [[nodiscard]] const vk::raii::Instance &instance() const { return m_instance; }
        vk::raii::Instance &instance() { return m_instance; }
    };
}
