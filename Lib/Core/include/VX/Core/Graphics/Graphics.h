#pragma once

#if VX_GRAPHICS_METAL
    #include <VX/Core/Graphics/API/Metal.h>
    namespace VX::Core::Graphics {
        namespace Current = API::Metal;
    }
#else
    #include <VX/Core/Graphics/API/Generic.h>
    namespace VX::Core::Graphics {
        namespace Current = API::Generic;
    }
#endif
