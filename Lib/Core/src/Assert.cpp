#include <VX/Core/Assert.h>

namespace VX::Core {
    void runtime_assert(const char *expr_str,
                        bool expr,
                        const char *file,
                        int line,
                        const std::string& message,
                        spdlog::logger &logger) {
        if (!expr) {
            logger.critical("Assertion failed: {}\n\tExpected: {}\n\tAt: {}:{}", message, expr_str, file, line);
            abort();
        }
    }
}
