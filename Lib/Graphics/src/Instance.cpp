#include <VX/Graphics/Instance.h>
#include <VX/Graphics/Private/InstanceData.h>

namespace VX::Graphics {
    Instance::Instance(std::unique_ptr<Private::InstanceData> instance_data)
            : m_instance_data(std::move(instance_data))
    {
    }
}
