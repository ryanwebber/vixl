#pragma once

#include <memory>
#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Copyable.h>

#include <VX/Graphics/Graphics.h>

namespace VX::Graphics {

    namespace Private {
        class InstanceData;
    }

    class Instance final : private std::enable_shared_from_this<Instance> {
        VX_MAKE_NONMOVABLE(Instance);
        VX_MAKE_NONCOPYABLE(Instance);
    private:
        std::unique_ptr<Private::InstanceData> m_instance_data;
    public:
        explicit Instance(std::unique_ptr<Private::InstanceData>);
        ~Instance() = default;

        Private::InstanceData& data() { return *m_instance_data; };
        const Private::InstanceData& data() const { return *m_instance_data; };
    };
}
