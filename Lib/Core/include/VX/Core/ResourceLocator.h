#pragma once

#include <filesystem>
#include <utility>

#include <VX/Copyable.h>

namespace VX::Core {
    class ResourceLocator final {
        VX_DEFAULT_MOVABLE(ResourceLocator);
        VX_DEFAULT_COPYABLE(ResourceLocator);
    private:
        std::filesystem::path m_resource_path;
    public:
        explicit ResourceLocator(const std::filesystem::path& path)
            : m_resource_path({ path })
        {
        }

        ~ResourceLocator() = default;

        [[nodiscard]] const std::filesystem::path& resource_path() const { return m_resource_path; }
        [[nodiscard]] std::filesystem::path get_named_resource(const std::filesystem::path& appending) const {
            return m_resource_path / appending;
        }
    };
}
