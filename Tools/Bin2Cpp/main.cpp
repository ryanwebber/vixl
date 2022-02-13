#include <filesystem>
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>

#include <VX/ArgParse/ArgParse.h>

static std::string s_input_file;
static std::string s_output_file;
static std::string s_name;
static std::string s_namespace;
static bool s_print_help = false;

static const std::vector<VX::ArgParse::OptionParser> s_option_parsers = {
        {
                "-i",
                "--input",
                2,
                [](const char* argp[]) { s_input_file = argp[1]; }
        },
        {
                "-o",
                "--output",
                2,
                [](const char* argp[]) { s_output_file = argp[1]; }
        },
        {
                "-n",
                "--name",
                2,
                [](const char* argp[]) { s_name = argp[1]; }
        },
        {
                "-s",
                "--namespace",
                2,
                [](const char* argp[]) { s_namespace = argp[1]; }
        },
        {
                "-h",
                "--help",
                1,
                [](const char* argp[]) { s_print_help = true; }
        },
};

static void print_help() {
    auto usage_string = ""
                        "Usage:\n"
                        "    bin2cpp --input <FILE> --output <FILE> --name <NAME>\n"
                        "    bin2cpp --help\n"
                        "\n"
                        "Generates C++ header files with the binary contents of a file packed into an array.\n"
                        "\n"
                        "Options:\n"
                        "  -i,--input               the input file\n"
                        "  -o,--output              the output file\n"
                        "  -n,--name                the name of the generated array\n"
                        "  -s,--namespace           the namespace to put the generated array in\n"
                        "  -h,--help                show this help message and exit.\n"
    ;

    std::cout << usage_string << std::endl;
}

int main (int argc, const char *argv[])
{
    bool parse_success = VX::ArgParse::parse(argc, argv, s_option_parsers);
    if (!parse_success || s_print_help) {
        print_help();
        return !parse_success;
    }

    if (s_input_file.empty() || s_output_file.empty()) {
        std::cout << "Both input and output file names must be specified." << std::endl << std::endl;
        print_help();
        return 1;
    }

    std::filesystem::path input_path = s_input_file;
    std::filesystem::path output_path = s_output_file;

    if (s_name.empty()) {
        auto input_file_name = input_path.filename().replace_extension();
        s_name = input_file_name;
    }

    std::ifstream input_stream(input_path, std::ios::binary);
    if (input_stream.fail() || !input_stream.is_open()) {
        std::cerr << "Unable to open input: " << input_path << std::endl;
        return 1;
    }

    std::ofstream output_stream(output_path);
    if (output_stream.fail() || !output_stream.is_open()) {
        std::cerr << "Unable to write to output: " << output_path << std::endl;
        return 1;
    }

    output_stream << "#include <vector>" << std::endl;
    output_stream << std::endl;

    if (!s_namespace.empty()) {
        output_stream << "namespace " << s_namespace << " {" << std::endl;
    }

    output_stream << "const std::vector<unsigned char> " << s_name << " = {" << std::endl;

    char buffer[16];
    while (input_stream.read(buffer, sizeof(buffer)) || input_stream.gcount() > 0) {
        output_stream << "    ";
        for (auto i = 0; i < input_stream.gcount(); i++) {
            auto byte = static_cast<unsigned char>(buffer[i]);
            output_stream << "0x" << std::setfill('0') << std::setw(2) << std::nouppercase << std::hex << static_cast<int>(byte);
            output_stream << ", ";
        }

        output_stream << std::endl;
    }

    output_stream << "};" << std::endl;

    if (!s_namespace.empty()) {
        output_stream << "}" << std::endl;
    }

    return 0;
}
