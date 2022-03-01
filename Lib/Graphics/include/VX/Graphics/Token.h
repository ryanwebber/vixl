#pragma once

#include <memory>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

namespace VX::Graphics {

    template <class T>
    class TokenLender;

    template <class T>
    class Token final {
        VX_DEFAULT_MOVABLE(Token);
        VX_MAKE_NONCOPYABLE(Token);
    private:
        std::unique_ptr<T> m_obj;
        std::weak_ptr<TokenLender<T>> m_lender;

    public:
        Token()
            : m_obj(nullptr)
            , m_lender({ })
        {}

        Token(std::unique_ptr<T> obj, std::weak_ptr<TokenLender<T>> lender)
            : m_obj(std::move(obj))
            , m_lender(lender)
        {
        }

        [[nodiscard]] bool is_valid() const { return m_obj != nullptr; }
        void return_token();

        ~Token() { return_token(); }

        T& operator*() { return *m_obj; }
        const T& operator*() const { return *m_obj; }

        T* operator->() { return m_obj.get(); }
        const T* operator->() const { return m_obj.get(); }

        friend class TokenLender<T>;
    };

    template <class T>
    class TokenLender final : public std::enable_shared_from_this<TokenLender<T>> {
    private:
        Token<T> m_token;

    public:
        explicit TokenLender(std::unique_ptr<T> data)
                : m_token(std::move(data), this->weak_from_this())
        {}

        [[nodiscard]] bool has_token() const { return m_token.is_valid(); }
        Token<T> borrow_token() { return std::move(m_token); }

        T &peek() { return *m_token; }
        const T &peek() const { return *m_token; }

        bool reclaim(Token<T> &token) {
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

        friend class Token<T>;
    };

    template<class T>
    void Token<T>::return_token() {
        if (auto lender = m_lender.lock()) {
            lender->reclaim(*this);
        }
    }
}
