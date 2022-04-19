#include <VX/Core/Assert.h>
#include <VX/Core/ResourceManager.h>

namespace VX::Core {
    Promise<AssetBundle> ResourceManager::load_asset_bundle(const std::filesystem::path &path, size_t offset, size_t size) {
        auto full_path = m_locator.get_named_resource(path);

        Log::debug("Attempting to load asset: {} (offset={} size={})", full_path.string(), offset, size);

        auto promise = FileSystem::read_file(m_executor, full_path, offset, size)
                .map<AssetBundle>([](FileSystem::ReadResult &result) {
                    if (!result)
                        VX_CORE_ASSERT(result.has_value(), "{}", result.error().description());

                    auto& byte_buffer = result.value();
                    Log::debug("Got asset with length={}", byte_buffer.size());

                    return AssetBundle(std::move(byte_buffer));
                });

        return std::move(promise);
    }
}
