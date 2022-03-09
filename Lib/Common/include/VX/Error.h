#pragma once

#include <exception>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <VX/Copyable.h>

namespace VX {

    class Error final : public std::exception {
        VX_DEFAULT_COPYABLE(Error);
        VX_DEFAULT_MOVABLE(Error);

    private:
        std::vector<std::string> m_context;

    public:
        explicit Error(const std::string_view& reason);
        ~Error() override = default;

        [[nodiscard]] const char *what() const noexcept override;

        [[nodiscard]] std::string description() const noexcept;

        template<typename... Args>
        Error with_context_format(std::string_view format, Args &&... args) noexcept;
    };

    template<typename... Args>
    Error Error::with_context_format(std::string_view format, Args &&... args) noexcept {
        auto context_string = fmt::vformat(format, fmt::make_format_args(args...));
        m_context.push_back(std::move(context_string));
        return std::move(*this);
    }

    template<typename... Args>
    Error make_error(std::string_view format, Args &&... args) noexcept {
        auto context_string = fmt::vformat(format, fmt::make_format_args(args...));
        return Error(context_string);
    }
}
