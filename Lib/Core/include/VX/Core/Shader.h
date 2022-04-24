#pragma once

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

#include <VX/Graphics/Graphics.h>
#include <VX/Core/Shader.h>

namespace VX::Core {
    class Shader final {
        VX_DEFAULT_MOVABLE(Shader);
        VX_DEFAULT_COPYABLE(Shader);
    private:
        Graphics::SharedHandle<Graphics::HandleType::Shader> m_handle;
    public:
        explicit Shader(Graphics::SharedHandle<Graphics::HandleType::Shader> handle)
            : m_handle(std::move(handle))
        {};

        ~Shader() = default;

        [[nodiscard]] const Graphics::ShaderHandle &handle() const { return *m_handle; }
    };
}
