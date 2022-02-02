#pragma once

#include <memory>
#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Copyable.h>

namespace VX::Graphics {

    namespace Private {
        class InstanceData;
    }

    struct PlatformData {
        void* native_window_handle { nullptr };
        std::vector<const char*> required_extensions { };
    };

    struct GraphicsInfo {
        PlatformData platform_data;
    };

    class Instance;
    std::shared_ptr<Instance> init(const GraphicsInfo&);

    class Instance final : private std::enable_shared_from_this<Instance> {
        VX_MAKE_NONMOVABLE(Instance);
        VX_MAKE_NONCOPYABLE(Instance);
    private:
        std::unique_ptr<Private::InstanceData> m_instance_data;
        explicit Instance(std::unique_ptr<Private::InstanceData>);
    public:
        ~Instance() = default;

        Private::InstanceData& data() { return *m_instance_data; };
        const Private::InstanceData& data() const { return *m_instance_data; };

        friend std::shared_ptr<Instance> init(const GraphicsInfo&);
    };
}
