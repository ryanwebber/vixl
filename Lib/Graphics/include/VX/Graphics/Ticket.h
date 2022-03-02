#pragma once

#include <memory>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

namespace VX::Graphics {

    template <class T>
    class TicketOwner;

    template <class T>
    class Ticket final {
        VX_DEFAULT_MOVABLE(Ticket);
        VX_MAKE_NONCOPYABLE(Ticket);
    private:
        std::unique_ptr<T> m_obj;
        std::weak_ptr<TicketOwner<T>> m_lender;

    public:
        Ticket()
            : m_obj(nullptr)
            , m_lender({ })
        {}

        Ticket(std::unique_ptr<T> obj, std::weak_ptr<TicketOwner<T>> lender)
            : m_obj(std::move(obj))
            , m_lender(lender)
        {
        }

        [[nodiscard]] bool is_valid() const { return m_obj != nullptr; }
        void return_ticket();

        ~Ticket() { return_ticket(); }

        T& operator*() { return *m_obj; }
        const T& operator*() const { return *m_obj; }

        T* operator->() { return m_obj.get(); }
        const T* operator->() const { return m_obj.get(); }

        friend class TicketOwner<T>;
    };

    template <class T>
    class TicketOwner final : public std::enable_shared_from_this<TicketOwner<T>> {
    private:
        Ticket<T> m_token;

    public:
        explicit TicketOwner(std::unique_ptr<T> data)
                : m_token(std::move(data), this->weak_from_this())
        {}

        [[nodiscard]] bool has_token() const { return m_token.is_valid(); }
        Ticket<T> borrow_token() { return std::move(m_token); }

        T &peek() { return *m_token; }
        const T &peek() const { return *m_token; }

        bool reclaim(Ticket<T> &token) {
            if (token.is_valid()) {
                if (auto lender = token.m_lender.lock()) {
                    if (lender.get() == this) {
                        m_token = std::move(token);
                        return true;
                    }
                }
            }

            return false;
        }

        explicit operator bool() const { return has_token(); }

        friend class Ticket<T>;
    };

    template<class T>
    void Ticket<T>::return_ticket() {
        if (auto lender = m_lender.lock()) {
            lender->reclaim(*this);
        }
    }
}
