#pragma once

#include <VX/Expected.h>

#include <VX/Core/RenderBuffer.h>
#include <VX/Graphics/Graphics.h>

namespace VX::Core {
    class RenderPass final {
    private:
        RenderBuffer m_buffer { };

    public:
        RenderPass() = default;
        ~RenderPass() = default;

        RenderBuffer &buffer() { return m_buffer; }
        [[nodiscard]] const RenderBuffer &buffer() const { return m_buffer; }
    };
}
