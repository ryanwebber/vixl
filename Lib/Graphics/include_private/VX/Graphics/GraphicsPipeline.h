#pragma once

#include <functional>
#include <utility>

#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/RenderContext.h>
#include <VX/Graphics/RenderPass.h>
#include <VX/Graphics/Swapchain.h>

namespace VX::Graphics {

    class Instance;
    class InstanceImpl;

    class GraphicsClient {
    public:
        virtual ~GraphicsClient() = default;
        virtual void do_render(const GraphicsStage&, GraphicsContext&) const = 0;
    };

    class GraphicsStageExecutor {
    private:
        GraphicsStage m_stage;

    public:
        explicit GraphicsStageExecutor(GraphicsStage stage)
                : m_stage(std::move(stage))
        {};

        virtual ~GraphicsStageExecutor() = default;

        virtual void prepare() { };
        virtual VX::Expected<void> execute(InstanceImpl&, GraphicsClient&) = 0;
        virtual void finish() { };


        [[nodiscard]] const GraphicsStage &stage() const { return m_stage; }
    };

    class GraphicsPipeline final {
        VX_DEFAULT_MOVABLE(GraphicsPipeline);
        VX_MAKE_NONCOPYABLE(GraphicsPipeline);
    private:
        class Client final : public GraphicsClient {
        private:
            const GraphicsDelegate &m_delegate;
        public:
            explicit Client(const GraphicsDelegate &delegate)
                    : m_delegate(delegate)
            {};

            ~Client() override = default;

            void do_render(const GraphicsStage &stage, GraphicsContext &context) const override {
                m_delegate.handle_stage(stage, context);
            }
        };

        std::vector<std::unique_ptr<GraphicsStageExecutor>> m_executors { };
    public:
        explicit GraphicsPipeline(std::vector<std::unique_ptr<GraphicsStageExecutor>> executors)
            : m_executors(std::move(executors))
        {};

        ~GraphicsPipeline() = default;

        VX::Expected<void> execute(InstanceImpl &instance, const GraphicsDelegate &delegate) const {
            for (const auto& executor : m_executors)
                executor->prepare();

            Client client(delegate);
            for (const auto& executor : m_executors) {
                executor->execute(instance, client);
            }

            for (const auto& executor : m_executors)
                executor->finish();

            return { };
        }
    };

    // Executors

    class RenderFrameStage final : public GraphicsStageExecutor {
        VX_MAKE_NONMOVABLE(RenderFrameStage);
        VX_MAKE_NONCOPYABLE(RenderFrameStage);
    private:
        static RenderContext render_context_from(const Swapchain::SwapState &);
    public:
        explicit RenderFrameStage(GraphicsStage stage)
            : GraphicsStageExecutor(std::move(stage))
        {};

        ~RenderFrameStage() override = default;

        VX::Expected<void> execute(InstanceImpl &instance, GraphicsClient &client) override;
    };
}
