#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <VX/Noncopyable.h>

#include <VX/Core/RenderContext.h>
#include <VX/Core/RenderLayer.h>
#include <VX/Core/RenderBuiltins.h>

namespace VX::Core {
    class SceneRenderer final : public RenderLayer {
        VX_MAKE_NONMOVABLE(SceneRenderer);
        VX_MAKE_NONCOPYABLE(SceneRenderer);
    private:
        std::shared_ptr<RenderBuiltins> m_builtins;
        std::vector<std::weak_ptr<RenderContext>> m_contexts;

    public:
        explicit SceneRenderer(std::shared_ptr<RenderBuiltins> builtins)
            : m_builtins(std::move(builtins))
        {}

        ~SceneRenderer() override = default;

        void on_initialize() override;
        void on_destroy() override;
        void on_update() override;
        void on_render() override;

        std::shared_ptr<RenderContext> create_render_context();
    };
}
