#include <cxxopts.hpp>

#include <iostream>

#include <flatmessage/compiler.hpp>

int main(int argc, char** argv)
{
    cxxopts::Options options("flatmessage_compiler", "flatmessage compiler");

    // clang-format off
    options.add_options()
        ("e,extension", "The extension used for the output file", cxxopts::value<std::string>())
        ("i,inputs", "The input file(s)", cxxopts::value<std::vector<std::string>>())
        ("t,template", "The template file", cxxopts::value<std::string>())
        ("o,outputDirectory", "The directory where all of your compiled flatmessage files will be written to", cxxopts::value<std::string>())
        ("m,mergeOutputs", "Merges all outputs into one big file", cxxopts::value<bool>()->default_value("false"))
        ;
    // clang-format on

    if (argc == 1 || (argc == 2 && (argv[1] == std::string{"--help"} || argv[1] == std::string{"-h"})))
    {
        std::cout << options.help();
        return 0;
    }

    options.parse_positional("input");

    try
    {
        auto result = options.parse(argc, argv);

        auto extension = result["e"].as<std::string>();
        auto inputs = result["i"].as<std::vector<std::string>>();
        auto tmplate = result["t"].as<std::string>();
        auto outDir = result["o"].as<std::string>();
        auto merge = result["m"].as<bool>();

        if (extension.empty() || inputs.empty() || tmplate.empty() || outDir.empty())
            return -1;

        std::vector<flatmessage::file_template_pair> files;
        for (auto&& input : inputs)
            files.emplace_back(flatmessage::file_template_pair{input, tmplate});

        auto flags = merge ? flatmessage::compiler_flags::merge_translation_units : flatmessage::compiler_flags::none;
        flatmessage::compiler compiler;
        compiler.compile_files(files, {1, outDir, extension, flags});
    }
    catch (std::exception& e)
    {
        std::cerr << "flatmessage_compiler:\n" << e.what() << '\n';
        return -2;
    }
    return 0;
}