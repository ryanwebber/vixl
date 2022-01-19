#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <VX/Expected.h>
#include <VX/Noncopyable.h>

#include <VX/Core/ResourceLocator.h>
#include <VX/Core/Async.h>

namespace VX::Core {

    class ResourceManager final {
        VX_MAKE_NONMOVABLE(ResourceManager);
        VX_MAKE_NONCOPYABLE(ResourceManager);
    private:
        ResourceLocator m_Locator;
        std::shared_ptr<EventLoop> m_EventLoop;

    public:
        ResourceManager(const std::filesystem::path& resource_path, std::shared_ptr<EventLoop> event_loop)
            : m_Locator(resource_path)
            , m_EventLoop(std::move(event_loop))
            {
            }

        ResourceManager(const ResourceLocator &resource_locator, std::shared_ptr<EventLoop> event_loop)
                : m_Locator({ resource_locator })
                , m_EventLoop(std::move(event_loop))
        {
        }

        ~ResourceManager() = default;

        [[nodiscard]] const ResourceLocator& GetLocator() const { return m_Locator; }
    };
}
