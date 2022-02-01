#pragma once

#if VX_PLATFORM_MACOS
    #include <VX/Core/Platform/MacOS/Platform_MacOS.h>
#elif VX_PLATFORM_GENERIC
    #include <VX/Core/Platform/Generic/Platform_Generic.h>
#else
    #error Unknown platform
#endif
