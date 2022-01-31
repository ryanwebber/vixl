#pragma once

#include <memory>
#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Copyable.h>

namespace VX::Graphics {

    namespace Private {
        class InstanceData;
    }

    class Instance;

    struct PlatformData {
        void* native_window_handle;
        std::vector<const char*> required_extensions;
    };

    struct GraphicsInfo {
        PlatformData platform_data;
    };

    std::shared_ptr<Instance> init(const GraphicsInfo&);

    class Instance final : private std::enable_shared_from_this<Instance> {
        VX_MAKE_NONMOVABLE(Instance);
        VX_MAKE_NONCOPYABLE(Instance);
    private:
        std::unique_ptr<Private::InstanceData> m_instance_data;
        explicit Instance(std::unique_ptr<Private::InstanceData>);
    public:
        ~Instance() = default;

        friend std::shared_ptr<Instance> init(const GraphicsInfo&);
    };
}
