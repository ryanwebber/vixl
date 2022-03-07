#pragma once

#include <VX/Graphics/APIObject.h>

namespace VX::Graphics {

    namespace Private {
        class GraphicsPipelineImpl;
    }

    class GraphicsPipeline final : public APIObject<Private::GraphicsPipelineImpl> {
    public:
        using APIObject<Private::GraphicsPipelineImpl>::APIObject;
    };
}
