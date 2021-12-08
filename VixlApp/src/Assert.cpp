#include <App/Logger.h>
#include <App/Dispatcher.h>

#include <App/Assert.h>

void RuntimeAssert(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
    if (!expr) {
        Logger::Core->critical("Assertion failed: {}\n\tExpected: {}\n\tAt: {}:{}",
                               msg,
                               expr_str,
                               file,
                               line);

        Dispatcher::Main().Terminate();
        abort();
    }
}
