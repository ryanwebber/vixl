#include <VX/Error.h>

#include <sstream>
#include <string>

namespace VX {

    Error::Error(const std::string_view& reason) {
        std::string reason_str(reason);
        m_context.push_back(std::move(reason_str));
    }

    const char *Error::what() const noexcept {
        return m_context.front().c_str();
    }

    std::string Error::description() const noexcept {
        std::stringstream ss;
        ss << "Caused by: " << m_context.front() << std::endl;
        for (auto i = 1; i < m_context.size(); i++) {
            ss << "    [" << i << "] " << m_context[i];
            if (i < m_context.size() - 1)
                ss << std::endl;
        }

        return ss.str();
    }
}
