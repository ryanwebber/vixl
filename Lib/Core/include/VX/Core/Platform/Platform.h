#pragma once

#include <bgfx/platform.h>

#if BX_PLATFORM_OSX
    #define VX_PLATFORM_OSX 1
    #include <VX/Core/Platform/API/OSX.h>
    namespace VX::Core::Platform {
        namespace Current = API::OSX;
    }
#elif BX_PLATFORM_WINDOWS
    #error Unsupported platform
#elif BX_PLATFORM_LINUX
    #error Unsupported platform
#else
    #include <VX/Core/Platform/API/Generic.h>
    namespace VX::Core::Platform {
        namespace Current = API::Generic;
    }
#endif
