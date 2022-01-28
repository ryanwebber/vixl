#include <iostream>
#include <optional>
#include <VX/ArgParse/ArgParse.h>

namespace VX::ArgParse {
    static bool parse(int argc, const char *argv[], const std::vector<OptionParser> &opts) {
        size_t i = 1;
        while (i < argc) {
            const char *arg = argv[i];
            if (arg[0] != '-') {
                std::cerr << "Unexpected argument: '" << arg << "'" << std::endl;
                return false;
            }

            auto found_opt = std::find_if(opts.begin(), opts.end(), [&](const auto &opt) {
               return opt.short_form == arg || opt.long_form == arg;
            });

            if (found_opt == opts.end()) {
                std::cerr << "Unknown option: '" << arg << "'" << std::endl;
                return false;
            }

            for (int a = 1; a < found_opt->argc; a++) {
                if (i + a >= argc || argv[i + a][0] == '-') {
                    std::cerr << "Option '" << arg << "' expects " << (found_opt->argc - 1) << " args but got " << (a - 1) << std::endl;
                    return false;
                }
            }

            found_opt->callback(&argv[i]);
            i += found_opt->argc;
        }

        return true;
    }
}
