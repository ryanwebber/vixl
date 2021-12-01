#include <Common/Error.h>

#include <sstream>
#include <string>

Error::Error(std::string_view reason) {
    std::string reason_str(reason);
    m_Context.push_back(std::move(reason_str));
}

Error::Error(Error &&other) noexcept
    : m_Context(std::move(other.m_Context))
    { }

const char* Error::what() const noexcept {
    return m_Context.front().c_str();
}

std::string Error::description() const noexcept {
    std::stringstream  ss;
    ss << "Caused by: " << m_Context.front() << std::endl;
    for (auto i = 1; i < m_Context.size(); i++) {
        ss << "    [" << i << "] " << m_Context[i];
        if (i < m_Context.size() - 1)
            ss << std::endl;
    }

    return ss.str();
}
