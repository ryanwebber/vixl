#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <Common/Expected.h>
#include <Common/Noncopyable.h>

#include <Core/ResourceLocator.h>
#include <Core/Async.h>

namespace Core {

    using namespace Async;

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
