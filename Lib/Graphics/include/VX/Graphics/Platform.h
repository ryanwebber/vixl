#pragma once

#include <vector>

namespace VX::Graphics {
    struct WindowSurfaceData {
    };

    struct PlatformData {
        std::vector<const char*> required_extensions { };
        WindowSurfaceData window_surface_data;
    };
}
