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
        ResourceLocator m_locator;
        std::shared_ptr<Executor> m_executor;

    public:
        ResourceManager(const std::filesystem::path& resource_path, std::shared_ptr<Executor> executor)
            : m_locator(resource_path)
            , m_executor(std::move(executor))
            {
            }

        ResourceManager(const ResourceLocator &resource_locator, std::shared_ptr<Executor> executor)
                : m_locator({ resource_locator })
                , m_executor(std::move(executor))
        {
        }

        ~ResourceManager() = default;

        [[nodiscard]] const ResourceLocator& locator() const { return m_locator; }
    };
}
