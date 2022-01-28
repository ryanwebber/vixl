#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace VX::ArgParse {
    struct OptionParser {
        const std::string_view short_form;
        const std::string_view long_form;
        std::size_t argc;
        std::function<void(const char* argp[])> callback;
    };

    static bool parse(int argc, const char *argv[], const std::vector<OptionParser> &opts);
}
