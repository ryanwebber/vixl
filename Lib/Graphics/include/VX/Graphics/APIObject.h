#pragma once

#include <memory>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

namespace VX::Graphics {
    template <class T>
    class APIObject {
        VX_DEFAULT_MOVABLE(APIObject);
        VX_MAKE_NONCOPYABLE(APIObject);
    private:
        std::unique_ptr<T> m_data;

    public:
        template <class ...Args>
        explicit APIObject(Args&& ...args)
            : m_data(std::make_unique<T>(std::forward<Args>(args)...))
        {}

        const T &impl() const { return *m_data; }
        T &impl() { return *m_data; }

        T& operator*() { return *m_data; }
        const T& operator*() const { return *m_data; }

        T* operator->() { return m_data.operator->(); }
        T const * operator->() const { return m_data.operator->(); }

    public:
        virtual ~APIObject() = default;
    };

    template <class T>
    class APIWrapper {
        VX_DEFAULT_MOVABLE(APIWrapper);
        VX_MAKE_NONCOPYABLE(APIWrapper);
    private:
        T m_obj;
    public:
        template <class ...Args>
        explicit APIWrapper(Args&& ...args)
            : m_obj(std::forward<Args>(args)...)
        {}

        explicit APIWrapper(T other)
            : m_obj(std::move(other))
        {}

        auto& operator*() { return *m_obj; }
        const auto& operator*() const { return *m_obj; }

        auto operator->() { return &m_obj; }
        auto operator->() const { return &m_obj; }
    };
}
