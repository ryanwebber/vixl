#include <VX/Core/Assert.h>

namespace VX::Core {
    void runtime_assert(const char *expr_str, bool expr, const char *file, int line, const char *msg,
                       spdlog::logger &logger) {
        if (!expr) {
            logger.critical("Assertion failed: {}\n\tExpected: {}\n\tAt: {}:{}", msg, expr_str, file, line);
            abort();
        }
    }
}
