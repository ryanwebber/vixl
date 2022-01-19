#include <corefoundation/CFBundle.h>
#include <limits.h>

namespace VX::Platforms::OSX {
    static std::filesystem::path GetResourceDirectory() {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);

        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)path, sizeof(path))) {
            abort();
        }

        return { path };
    }
}
