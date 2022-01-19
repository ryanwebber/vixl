#include <filesystem>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

struct Asset {
    struct MetadataEntry {
        enum class Type { String, Int, Bool };

        std::string key;
        std::string value;
        Type type { Type::String };
    };

    std::string name;
    std::string source;
    std::vector<MetadataEntry> metadata;

    std::size_t packed_offset { 0 };
    std::size_t packed_length { 0 };
};

struct OptionParser {
    const std::string short_form;
    const std::string long_form;
    size_t argc;
    std::function<void(const char* argp[])> callback;
};

static std::string s_path_prefix = std::filesystem::current_path().string();
static std::string s_input_manifest;
static std::string s_output_package;
static std::string s_output_header;
static std::string s_namespace("Assets");
static std::filesystem::path s_cwd(std::filesystem::current_path());
static bool s_print_help = false;

static OptionParser s_option_parsers[] = {
        {
            "-i",
            "--input",
            2,
            [](const char* argp[]) { s_input_manifest = argp[1]; }
        },
        {
            "-o",
            "--output",
            2,
            [](const char* argp[]) { s_output_package = argp[1]; }
        },
        {
            "-l",
            "--listing",
            2,
            [](const char* argp[]) { s_output_header = argp[1]; }
        },
        {
            "-n",
            "--namespace",
            2,
            [](const char* argp[]) { s_namespace = argp[1]; }
        },
        {
            "-c",
            "--cwd",
            2,
            [](const char* argp[]) { s_cwd = argp[1]; }
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
                        "    asset-pack --input <FILE> --output <FILE> --listing <FILE> \n"
                        "    asset-pack -h | --help\n"
                        "\n"
                        "Generates asset manifest files.\n"
                        "\n"
                        "Options:\n"
                        "  -i,--input               the input asset manifest\n"
                        "  -o,--output              the packed asset output bundle file\n"
                        "  -l,--listing             the output header listing of asset metadata\n"
                        "  -c,--cwd                 the current working directory relative to asset manifest source paths\n"
                        "  -n,--namespace           the namespace to put asset listing data in\n"
                        "  -h,--help                show this help message and exit.\n"
                        ;

    std::cout << usage_string << std::endl;
}

static bool parse_args(int argc, const char *argv[]) {
    size_t i = 1;
    while (i < argc) {
        const char *arg = argv[i];
        if (arg[0] != '-') {
            std::cerr << "Unexpected argument: '" << arg << "'" << std::endl;
            return false;
        }

        OptionParser *found_parser = nullptr;
        for (auto &&parser: s_option_parsers) {
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
    bool parse_success = parse_args(argc, argv);
    if (!parse_success || s_print_help) {
        print_help();
        return !parse_success;
    }

    if (s_input_manifest.empty() || s_output_package.empty() || s_output_header.empty()) {
        print_help();
        return 1;
    }

    // Read the input manifest
    std::ifstream input_manifest(s_input_manifest);
    if (input_manifest.fail() || !input_manifest.is_open()) {
        std::cerr << "Unable to open asset manifest" << std::endl;
        return 2;
    }

    nlohmann::json input_json;
    std::vector<Asset> assets;
    try {
        input_manifest >> input_json;
        for (auto& element : input_json["resources"].items()) {
            const auto &value = element.value();

            Asset asset;
            asset.name = value["name"];
            asset.source = value["src"];

            for(auto& metadata_item : value["metadata"].items()) {
                Asset::MetadataEntry entry;
                entry.key = fmt::format("{}", metadata_item.key());
                if (metadata_item.value().is_number()) {
                    entry.value = fmt::format("{}", metadata_item.value().get<int>());
                    entry.type = Asset::MetadataEntry::Type::Int;
                } else if (metadata_item.value().is_boolean()) {
                    entry.value = fmt::format("{}", metadata_item.value().get<bool>());
                    entry.type = Asset::MetadataEntry::Type::Bool;
                } else {
                    entry.value = fmt::format("\"{}\"", metadata_item.value());
                    entry.type = Asset::MetadataEntry::Type::String;
                }

                asset.metadata.push_back(entry);
            }

            assets.push_back(asset);
        }
    } catch (const std::exception& e) {
        std::cerr << "Unable to parse input manifest: " << e.what() << std::endl;
        return 3;
    }

    // Write the packed output file
    std::ofstream output_bundle(s_output_package);
    if (output_bundle.fail() || !output_bundle.is_open()) {
        std::cerr << "Unable to write to packed output file" << std::endl;
        return 2;
    }

    size_t total_bytes_written = 0;
    for (auto& asset : assets) {
        auto packed_offset = total_bytes_written;
        auto filepath = s_cwd / asset.source;

        std::cout << "Writing '" << filepath << "' >> '" << s_output_package << "'..." << std::endl;

        std::ifstream asset_source(filepath);
        if (asset_source.fail() || !asset_source.is_open()) {
            std::cerr << "Unable to open asset file: " << filepath << std::endl;
            return 2;
        }

        output_bundle << asset_source.rdbuf();
        total_bytes_written = output_bundle.tellp();

        asset.packed_offset = packed_offset;
        asset.packed_length = total_bytes_written - packed_offset;
    }

    // Write the header listing
    std::ofstream output_header(s_output_header);
    if (output_header.fail() || !output_header.is_open()) {
        std::cerr << "Unable to write to header listing file" << std::endl;
        return 2;
    }

    output_header << "#include <string>" << std::endl;
    output_header << "#include <VX/Core/PackedAsset.h>" << std::endl;
    output_header << "namespace " << s_namespace << " {" << std::endl;
    for (auto& asset : assets) {
        std::cout << "Asset '" << asset.name << "': " << asset.packed_offset << "+" << asset.packed_length << std::endl;
        output_header << "namespace " << asset.name << " {" << std::endl;
        output_header << "  static const VX::PackedAsset asset = {" << std::endl;
        output_header << "    .resource_name = \"" << asset.name << "\"," << std::endl;
        output_header << "    .offset = " << asset.packed_offset << "," << std::endl;
        output_header << "    .size = " << asset.packed_length << "," << std::endl;
        output_header << "  };" << std::endl;
        output_header << "  namespace metadata {" << std::endl;

        for (auto& entry : asset.metadata) {
            output_header << "    static const ";
            switch (entry.type) {
                case Asset::MetadataEntry::Type::Bool:
                    output_header << "bool";
                    break;
                case Asset::MetadataEntry::Type::Int:
                    output_header << "int";
                    break;
                case Asset::MetadataEntry::Type::String:
                    output_header << "std::string";
                    break;
            }
            output_header << " " << entry.key << " = " << entry.value << ";" << std::endl;
        }

        output_header << "  }" << std::endl;
        output_header << "}" << std::endl;
    }

    output_header << "}" << std::endl;

    return 0;
}
