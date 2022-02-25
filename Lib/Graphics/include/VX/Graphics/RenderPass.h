#pragma once

#include <VX/Graphics/APIObject.h>
#include <VX/Graphics/CommandRecorder.h>

namespace VX::Graphics {

    namespace Private {
        class RenderPassImpl;
    }

    class RenderPass final: public APIObject<Private::RenderPassImpl> {
    public:
        using APIObject<Private::RenderPassImpl>::APIObject;

        [[nodiscard]] const CommandRecorder& command_recorder() const;
        CommandRecorder& command_recorder();
    };
}
