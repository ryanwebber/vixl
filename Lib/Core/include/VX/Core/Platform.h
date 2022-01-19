#pragma once

#include <bgfx/platform.h>

#if BX_PLATFORM_OSX
    #define VX_PLATFORM_OSX 1
    #include <VX/Core/Platforms/OSX.h>
    namespace VX::Core {
        namespace Platform = Platforms::OSX;
    }
#elif BX_PLATFORM_WINDOWS
    #error Unsupported platform
#elif BX_PLATFORM_LINUX
    #error Unsupported platform
#else
    #include <VX/Core/Platforms/Generic.h>
    namespace VX::Core {
        namespace Platform = Platforms::Generic;
    }
#endif
