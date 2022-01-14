#pragma once

#include <filesystem>
#include <utility>

#include <Common/Copyable.h>

namespace Core {
    class ResourceLocator final {
        VX_DEFAULT_MOVABLE(ResourceLocator);
        VX_DEFAULT_COPYABLE(ResourceLocator);
    private:
        std::filesystem::path m_ResourcePath;
    public:
        explicit ResourceLocator(const std::filesystem::path& path)
            : m_ResourcePath({ path })
            {
            }

        ~ResourceLocator() = default;

        [[nodiscard]] const std::filesystem::path& GetResourcePath() const { return m_ResourcePath; }
        std::filesystem::path GetResource(const std::filesystem::path& appending) const {
            return m_ResourcePath / appending;
        }
    };
}
