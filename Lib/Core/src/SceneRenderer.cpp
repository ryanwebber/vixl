#include <bx/math.h>
#include <glm/gtc/type_ptr.hpp>

#include <VX/Core/RenderPass.h>
#include <VX/Core/SceneRenderer.h>
#include <VX/Core/Logger.h>

namespace VX::Core {

    void SceneRenderer::on_initialize() {
        if (m_builtins == nullptr)
            m_builtins = std::make_shared<RenderBuiltins>();
    }

    void SceneRenderer::on_destroy() {
    }

    void SceneRenderer::on_update() {
        auto it = std::remove_if(m_contexts.begin(), m_contexts.end(), [](const std::weak_ptr<RenderContext>& context) {
            return context.expired();
        });

        m_contexts.erase(it, m_contexts.end());
    }

    void SceneRenderer::on_render() {
        RenderPass rp;
        for (auto&& weak_context : m_contexts) {
            if (!weak_context.expired()) {
                auto context = weak_context.lock();

                auto view_matrix_ptr = glm::value_ptr(context->view_projection().view_matrix);
                auto proj_matrix_ptr = glm::value_ptr(context->view_projection().projection_matrix);
                bgfx::setViewTransform(0, view_matrix_ptr, proj_matrix_ptr);

                for (auto &command : context->buffer().commands()) {
                    rp.submit(command);
                }

                // Wipe all commands and reset for the next frame
                context->reset();
            }
        }
    }

    std::shared_ptr<RenderContext> SceneRenderer::create_render_context() {
        auto target = std::make_shared<RenderContext>(m_builtins);
        m_contexts.push_back(target);
        return target;
    }
}
