#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/CommandBuffer.h>
#include <VX/Graphics/GraphicsPipeline.h>

namespace VX::Graphics {

    namespace Private {
        class CommandRecorderImpl;
    }

    class CommandRecorder final: public APIObject<Private::CommandRecorderImpl> {
    public:
        using APIObject<Private::CommandRecorderImpl>::APIObject;

        void bind(const GraphicsPipeline&) const;
        void draw() const;
    };
}
