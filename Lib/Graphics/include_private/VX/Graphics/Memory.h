#pragma once

#include <vector>

#include <VX/Noncopyable.h>
#include <VX/Graphics/Assert.h>
#include <VX/Graphics/Graphics.h>
#include <VX/Graphics/RenderTarget.h>
#include <VX/Graphics/Vulkan.h>

namespace VX::Graphics {

    // Arena allocation for these types of resources:
    //  1. Instance-based lifetime
    //  2. Allocated external objects

    template <class T>
    class InstancePtr final {
        VX_DEFAULT_MOVABLE(InstancePtr);
        VX_DEFAULT_COPYABLE(InstancePtr);
    private:
        std::weak_ptr<T> m_ptr;
    public:
        InstancePtr(std::weak_ptr<T> ptr)
            : m_ptr(ptr)
        {}

        [[nodiscard]] const std::weak_ptr<T> &weak() const {
            return m_ptr;
        }

        T& operator*() {
            if (auto shared = m_ptr.lock())
                return *shared;

            VX_GRAPHICS_ASSERT_NOT_REACHED();
        }

        const T& operator*() const {
            if (auto shared = m_ptr.lock())
                return *shared;

            VX_GRAPHICS_ASSERT_NOT_REACHED();
        }

        T* operator->() { return &(**this); }
        const T* operator->() const { return &(**this); }

        ~InstancePtr() = default;
    };

    class Allocator final {
        VX_DEFAULT_MOVABLE(Allocator);
        VX_MAKE_NONCOPYABLE(Allocator);
    private:
        std::vector<std::shared_ptr<void>> m_allocations;
    public:
        explicit Allocator() = default;
        ~Allocator() = default;

        template <class T, class ...Args>
        InstancePtr<T> allocate(Args&&...args) {
            auto shared_ptr = std::make_shared<T>(std::forward<Args>(args)...);
            VX_GRAPHICS_ASSERT(shared_ptr != nullptr, "Failed to allocate");
            m_allocations.push_back(shared_ptr);
            return InstancePtr<T>(shared_ptr);
        }

        template <class T>
        void deallocate(const InstancePtr<T> &ptr) {
            if (std::shared_ptr<void> shared_instance = ptr.weak().lock()) {
                const auto &begin = m_allocations.begin();
                const auto &end = m_allocations.end();
                m_allocations.erase(std::remove(begin, end, shared_instance), end);
            }
        }
    };

    template <class T>
    class HandleAllocator final {
        VX_DEFAULT_MOVABLE(HandleAllocator);
        VX_MAKE_NONCOPYABLE(HandleAllocator);
    private:
        Identifier m_next_identifier { 1 };
        std::unordered_map<Identifier, InstancePtr<T>> m_lookup_table { };
        std::shared_ptr<Allocator> m_allocator;
    public:
        explicit HandleAllocator(std::shared_ptr<Allocator> backing_allocator)
            : m_allocator(std::move(backing_allocator))
        {};

        ~HandleAllocator() {
            for (const auto& [_, ptr] : m_lookup_table) {
                m_allocator->deallocate(ptr);
            }
        }

        template <HandleType H, class ...Args>
        Handle<H> allocate(Args&&...args) {
            auto ptr = m_allocator->allocate<T>(std::forward<Args>(args)...);
            auto identifier = m_next_identifier++;
            m_lookup_table.try_emplace(identifier, ptr);
            return Handle<H>(identifier);
        }

        template <HandleType H>
        void destroy(Handle<H> handle) {
            auto itr = m_lookup_table.find(handle.identifier());
            if (itr != m_lookup_table.end()) {
                auto instance_ptr = itr->second;
                m_allocator->template deallocate(instance_ptr);
                m_lookup_table.erase(itr);
            }
        }

        template <HandleType H>
        T &lookup(Handle<H> handle) {
            auto itr = m_lookup_table.find(handle.identifier());
            VX_GRAPHICS_ASSERT(itr != m_lookup_table.end(), "Handle not found in resource manager: {}", handle.identifier());
            return *(itr->second);
        }

        template <HandleType H>
        const T &lookup(Handle<H> handle) const {
            auto itr = m_lookup_table.find(handle.identifier());
            VX_GRAPHICS_ASSERT(itr != m_lookup_table.end(), "Handle not found in resource manager: {}", handle.identifier());
            return *(itr->second);
        }
    };
}
