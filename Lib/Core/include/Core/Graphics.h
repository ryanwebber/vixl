#pragma once

#if VX_GRAPHICS_METAL
    #include <Core/Graphics/Metal.h>
    namespace Core {
        namespace GraphicsAPI = GraphicsAPIs::Metal;
    }
#else
    #include <Core/Graphics/Generic.h>
    namespace Core {
        namespace Graphics = Graphics::Generic;
    }
#endif
