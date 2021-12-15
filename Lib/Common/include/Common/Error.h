#pragma once

#include <exception>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "Noncopyable.h"

namespace Common {

    class Error final : public std::exception {
    VX_MAKE_NONCOPYABLE(Error);

    private:
        std::vector<std::string> m_Context;

    public:
        explicit Error(std::string_view reason);

        Error(Error &&other) noexcept;

        ~Error() override = default;

        [[nodiscard]] const char *what() const noexcept override;

        [[nodiscard]] std::string description() const noexcept;

        template<typename... Args>
        Error with_context_format(std::string_view format, Args &&... args) noexcept;
    };

    template<typename... Args>
    Error Error::with_context_format(std::string_view format, Args &&... args) noexcept {
        auto context_string = fmt::vformat(format, fmt::make_format_args(args...));
        m_Context.push_back(std::move(context_string));
        return std::move(*this);
    }
}