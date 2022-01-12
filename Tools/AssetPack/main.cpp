#include <functional>
#include <iostream>
#include <string>

struct OptionParser {
    const std::string short_form;
    const std::string long_form;
    size_t argc;
    std::function<void(const char* argp[])> callback;
};

static std::string s_PathPrefix;
static std::string s_InputManifest;
static std::string s_OutputManifest;
static std::string s_OutputHeader;
static bool s_PrintHelp;

static OptionParser s_OptionParsers[] = {
        {
            "-i",
            "--input-manifest",
            2,
            [](const char* argp[]) { s_InputManifest = argp[1]; }
        },
        {
            "-o",
            "--output-manifest",
            2,
            [](const char* argp[]) { s_OutputManifest = argp[1]; }
        },
        {
            "-p",
            "--path-prefix",
            2,
            [](const char* argp[]) { s_PathPrefix = argp[1]; }
        },
        {
            "-l",
            "--output-header",
            2,
            [](const char* argp[]) { s_OutputHeader = argp[1]; }
        },
        {
            "-h",
            "--help",
            1,
            [](const char* argp[]) { s_PrintHelp = true; }
        },
};

static void PrintHelp() {
    auto usage_string = ""
                        "Usage:\n"
                        "    asset-pack --input-manifest <FILE> --output-manifest <FILE> --output-header <FILE> --path-prefix <FILE>\n"
                        "    asset-pack -h | --help\n"
                        "\n"
                        "Generates asset manifest files.\n"
                        "\n"
                        "Options:\n"
                        "  -i,--input-manifest      the input asset manifest\n"
                        "  -o,--output-manifest     the adjusted output asset manifest\n"
                        "  -l,--output-header       the output header listing of asset metadata\n"
                        "  -p,--path-prefix         the path prefix to prepend to asset paths\n"
                        "  -h,--help                show this help message and exit.\n"
                        ;

    std::cout << usage_string << std::endl;
}

static bool ParseArgs(int argc, const char *argv[]) {
    size_t i = 1;
    while (i < argc) {
        const char *arg = argv[i];
        if (arg[0] != '-') {
            std::cerr << "Unexpected argument: '" << arg << "'" << std::endl;
            return false;
        }

        OptionParser *found_parser = nullptr;
        for (auto &&parser: s_OptionParsers) {
            if (parser.short_form == arg || parser.long_form == arg) {
                found_parser = &parser;
                break;
            }
        }

        if (found_parser == nullptr) {
            std::cerr << "Unknown option: '" << arg << "'" << std::endl;
            return false;
        }

        for (int a = 1; a < found_parser->argc; a++) {
            if (i + a >= argc || argv[i + a][0] == '-') {
                std::cerr << "Option '" << arg << "' expects " << (found_parser->argc - 1) << " args but got " << (a - 1) << std::endl;
                return false;
            }
        }

        found_parser->callback(&argv[i]);
        i += found_parser->argc;
    }

    return true;
}

int main (int argc, const char *argv[]) {
    bool parse_success = ParseArgs(argc, argv);
    if (!parse_success || s_PrintHelp) {
        PrintHelp();
        return !parse_success;
    }

    std::cout << "Prefix: " << s_PathPrefix << std::endl;
    return 0;
}
