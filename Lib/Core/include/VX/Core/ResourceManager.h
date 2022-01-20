#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <VX/Expected.h>
#include <VX/Noncopyable.h>

#include <VX/Core/AssetBundle.h>
#include <VX/Core/ResourceLocator.h>
#include <VX/Core/Async.h>

namespace VX::Core {

    class ResourceManager final {
        VX_MAKE_NONMOVABLE(ResourceManager);
        VX_MAKE_NONCOPYABLE(ResourceManager);
    private:
        std::shared_ptr<Executor> m_executor;
        ResourceLocator m_locator;

    public:
        ResourceManager(std::shared_ptr<Executor> executor, const std::filesystem::path& resource_path)
                : m_executor(std::move(executor))
                , m_locator(resource_path)
        {
        }

        ResourceManager(std::shared_ptr<Executor> executor, ResourceLocator resource_locator)
                : m_executor(std::move(executor))
                , m_locator(std::move(resource_locator))
        {
        }

        ~ResourceManager() = default;

        [[nodiscard]] const ResourceLocator& locator() const { return m_locator; };
        Promise<AssetBundle> load_asset_bundle(const std::filesystem::path &path, size_t offset, size_t size);
    };
}
