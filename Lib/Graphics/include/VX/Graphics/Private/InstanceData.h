#pragma once

#include <memory>
#include <vector>

#include <VX/Graphics/Private/Vulkan.h>

namespace VX::Graphics::Private {
    class InstanceData {
    private:
        vk::raii::Context m_context;
        vk::raii::Instance m_instance;
        std::vector<std::shared_ptr<void>> m_resources;

    public:
        InstanceData(vk::raii::Context context, vk::raii::Instance instance, std::vector<std::shared_ptr<void>> resources)
                : m_context(std::move(context))
                , m_instance(std::move(instance))
                , m_resources(std::move(resources))
        {}

        ~InstanceData() = default;

        [[nodiscard]] const vk::raii::Instance &instance() const { return m_instance; }
        vk::raii::Instance &instance() { return m_instance; }
    };
}
