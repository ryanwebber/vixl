#pragma once

#include <VX/Platform/ResourceLocator.h>

namespace VX::Platform::Abstraction {

    class FileSystem {
    protected:
        FileSystem() = default;

    public:
        ~FileSystem() = default;

        [[nodiscard]] virtual const ResourceLocator& resource_locator() const = 0;
    };
}
