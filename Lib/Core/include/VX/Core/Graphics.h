#pragma once

#if VX_GRAPHICS_METAL
    #include <VX/Core/Graphics/Metal.h>
    namespace VX::Core {
        namespace GraphicsAPI = GraphicsAPIs::Metal;
    }
#else
    #include <VX/Core/Graphics/Generic.h>
    namespace VX::Core {
        namespace Graphics = Graphics::Generic;
    }
#endif
