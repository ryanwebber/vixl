#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include <VX/Error.h>
#include <VX/Expected.h>
#include <VX/Noncopyable.h>

namespace VX::Graphics {

    class InstanceImpl;
    
    class Instance final {
    private:
        std::shared_ptr<InstanceImpl> m_impl;
    public:
        Instance(std::shared_ptr<InstanceImpl>);

        Instance(Instance&&) noexcept;
        Instance& operator=(Instance&&) noexcept;

        Instance(const Instance&) noexcept;
        Instance& operator=(const Instance&) noexcept;
        
        ~Instance();

        const InstanceImpl* operator->() const;
        InstanceImpl* operator->();

        const InstanceImpl &operator*() const;
        InstanceImpl &operator*();
    };

    struct GraphicsInfo {
        std::vector<const char*> required_extensions { };
        glm::vec<2, int> framebuffer_dimensions { };
    };

    enum class HandleType {
        CommandBuffer,
        GraphicsPipeline,
        RenderContext,
        RenderTarget,
    };

    using HandleIdentifier = uint32_t;

    template <HandleType T>
    class Handle final {
        VX_MAKE_NONCOPYABLE(Handle);
    private:
        HandleIdentifier m_identifier;
        std::shared_ptr<Instance> m_instance;

    public:
        Handle(HandleIdentifier, std::shared_ptr<Instance>);

        Handle(Handle<T>&&) noexcept;
        Handle<T>& operator=(Handle<T>&&) noexcept;

        ~Handle();

        [[nodiscard]] HandleIdentifier identifier() const { return m_identifier; }
        [[nodiscard]] bool is_valid() const { return m_instance != nullptr; };
    };


    using CommandBufferHandle = Handle<HandleType::CommandBuffer>;
    using GraphicsPipelineHandle = Handle<HandleType::GraphicsPipeline>;
    using RenderContextHandle = Handle<HandleType::RenderContext>;
    using RenderTargetHandle = Handle<HandleType::RenderTarget>;

    template<> class Handle<HandleType::CommandBuffer>;
    template<> class Handle<HandleType::GraphicsPipeline>;
    template<> class Handle<HandleType::RenderContext>;
    template<> class Handle<HandleType::RenderTarget>;

    struct SwapState {
        std::shared_ptr<RenderContextHandle> context;
        std::shared_ptr<RenderTargetHandle> frame_buffer;
        std::shared_ptr<CommandBufferHandle> command_buffer;
    };

    VX::Expected<void> begin_render_pass(const Instance&, const RenderContextHandle&, const RenderTargetHandle&, const CommandBufferHandle&);
    VX::Expected<void> end_render_pass(const Instance&);

    VX::Expected<SwapState> begin_frame(const Instance&);
    VX::Expected<void> end_frame(const Instance&);

    VX::Expected<GraphicsPipelineHandle> create_graphics_pipeline(const Instance&);

    void Bind(const Instance&, const GraphicsPipelineHandle&);
    void Draw(const Instance&);
}
