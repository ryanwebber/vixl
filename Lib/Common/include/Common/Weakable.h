#pragma mark

#include <memory>

namespace Common {
    template<typename T>
    class Weakable : public std::enable_shared_from_this<T> {
    protected:
        Weakable() = default;
        std::weak_ptr<T> WeakSelf() noexcept {
            return this->weak_from_this();
        }

        std::shared_ptr<T> SharedSelf() {
            return this->shared_from_this();
        }

    public:
        ~Weakable() = default;
    };
}
