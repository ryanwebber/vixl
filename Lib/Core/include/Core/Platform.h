#pragma once

#include <bgfx/platform.h>

#if BX_PLATFORM_OSX
    #include <Core/Platforms/OSX.h>
    namespace Core {
        namespace Platform = Platforms::OSX;
    }
#elif BX_PLATFORM_WINDOWS
    #error Unsupported platform
#elif BX_PLATFORM_LINUX
    #error Unsupported platform
#else
    #include <Core/Platforms/Generic.h>
    namespace Core {
        namespace Platform = Platforms::Generic;
    }
#endif
