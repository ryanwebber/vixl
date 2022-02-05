#pragma once

#include <corefoundation/CFBundle.h>

#include <climits>
#include <filesystem>

#include <VX/Platform/Abstraction/FileSystem.h>

namespace VX::Platform::Private {
    class MacOSBundleFileSystem: public Abstraction::FileSystem {
    private:
        ResourceLocator m_resource_locator;

        MacOSBundleFileSystem();
    public:
        [[nodiscard]] const ResourceLocator& resource_locator() const override
        {
            return m_resource_locator;
        }

        static const Abstraction::FileSystem& instance() {
            static MacOSBundleFileSystem s_instance;
            return s_instance;
        }
    };

    std::filesystem::path get_resource_directory() {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)path, sizeof(path))) {
            abort();
        }

        CFRelease(resourceURL);

        return { path };
    }

    MacOSBundleFileSystem::MacOSBundleFileSystem()
        : m_resource_locator(get_resource_directory())
    {
    }
}
