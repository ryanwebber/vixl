#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

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
        explicit Instance(std::shared_ptr<InstanceImpl>);

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
    class Handle;

    template <HandleType H>
    class ResourceDeallocator {
    public:
        virtual ~ResourceDeallocator() = default;
        virtual void destroy_resource(Handle<H> &handle) = 0;
    };

    template <class T, HandleType H>
    class ResourceAllocator: public std::enable_shared_from_this<ResourceAllocator<T, H>>, public ResourceDeallocator<H> {
    private:
        HandleIdentifier m_index { 0 };
        std::unordered_map<HandleIdentifier, T> m_resources { };

        ResourceAllocator() = default;

    public:
        ~ResourceAllocator() = default;

        void destroy_resource(Handle<H> &handle) override;

        Handle<H> create_resource(const std::function<T()> &);
        Handle<H> create_resource(T);

        static std::shared_ptr<ResourceAllocator<T, H>> create_shared() {
            return std::shared_ptr<ResourceAllocator<T, H>>(new ResourceAllocator<T, H>());
        }
    };

    template <HandleType H>
    class Handle final {
        VX_MAKE_NONCOPYABLE(Handle);
    private:
        HandleIdentifier m_identifier;
        std::weak_ptr<ResourceDeallocator<H>> m_deallocator;

    public:
        Handle(HandleIdentifier identifier, std::weak_ptr<ResourceDeallocator<H>> deallocator)
            : m_identifier(identifier)
            , m_deallocator(std::move(deallocator))
        {}

        Handle(Handle<H>&&) noexcept = default;
        Handle<H>& operator=(Handle<H>&&) noexcept = default;

        void invalidate() noexcept { m_identifier = false; }

        [[nodiscard]] HandleIdentifier identifier() const { return m_identifier; }
        [[nodiscard]] bool is_valid() const { return m_identifier && !m_deallocator.expired(); };

        ~Handle() {
            if (auto deallocator = m_deallocator.lock())
                deallocator->destroy_resource(*this);

            invalidate();
        }
    };

    template<class T, HandleType H>
    void ResourceAllocator<T, H>::destroy_resource(Handle<H> &handle)
    {
        m_resources.erase(handle.identifier());
        handle.invalidate();
    }

    template<class T, HandleType H>
    Handle<H> ResourceAllocator<T, H>::create_resource(const std::function<T()> &fn)
    {
        return create_resource(fn());
    }

    template<class T, HandleType H>
    Handle<H> ResourceAllocator<T, H>::create_resource(T t)
    {
        auto identifier = ++m_index;
        m_resources.insert_or_assign(identifier, std::move(t));
        return Handle<H>(identifier, this->shared_from_this());
    }

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
