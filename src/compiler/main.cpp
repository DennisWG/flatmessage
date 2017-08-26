#include <cxxopts.hpp>

#include <iostream>

#include <flatmessage/compiler.hpp>

cxxopts::Options ParseCommandLine(int argc, char** argv)
{
    cxxopts::Options options("flatmessage_compiler", "A compiler for flatmessages");

    // clang-format off
    options.add_options()
        ("e,extension", "The extension used for the output file", cxxopts::value<std::string>())
        ("i,input", "The input file", cxxopts::value<std::vector<std::string>>())
        ("t,template", "The template file", cxxopts::value<std::string>())
        ("w,workdir", "This is the root directory of all your flatmessage files", cxxopts::value<std::string>())
        ;
    // clang-format on

    options.parse_positional("input");

    options.parse(argc, argv);

    return options;
}

int main(int argc, char** argv)
{
    auto options = ParseCommandLine(argc, argv);

    try
    {
        auto extension = options["e"].as<std::string>();
        auto inputs = options["i"].as<std::vector<std::string>>();
        auto tmplate = options["t"].as<std::string>();
        auto workdir = options["w"].as<std::string>();

        std::vector<flatmessage::file_template_pair> files;
        for (auto&& input : inputs)
            files.emplace_back(flatmessage::file_template_pair{input, tmplate});

        flatmessage::compiler compiler;
        compiler.compile_files(files, {1, workdir, extension});
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
        return -1;
    }

    return 0;
}