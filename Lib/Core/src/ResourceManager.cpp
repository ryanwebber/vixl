#include <Core/ResourceManager.h>

namespace Core {
    FileReadTask ResourceManager::LoadAsset(const std::string_view &bundle_name, size_t offset, size_t length) {
        auto bundle_path = std::filesystem::path(bundle_name);
        auto file_path = m_Locator.GetResource(bundle_path);

        FileReadTask task(m_EventLoop, file_path, offset, length);
        return std::move(task);
    }
}
