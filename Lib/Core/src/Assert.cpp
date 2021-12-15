
#include <Core/Dispatcher.h>
#include <Core/Assert.h>

namespace Core {
    void RuntimeAssert(const char *expr_str, bool expr, const char *file, int line, const char *msg,
                       spdlog::logger &logger) {
        if (!expr) {
            logger.critical("Assertion failed: {}\n\tExpected: {}\n\tAt: {}:{}", msg, expr_str, file, line);

            Dispatcher::Main().Terminate();
            abort();
        }
    }
}
