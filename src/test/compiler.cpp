/*
Copyright (c) 2016 Dennis Werner Garske (DWG)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "testing.hpp"

#include <testinator.h>

#include <boost/range/iterator_range.hpp>

#include <flatmessage/compiler.hpp>

namespace fs = boost::filesystem;

using input_expect_pair = std::pair<fs::path, std::string>;

const fs::path working_folder = fs::current_path() / "compiler_expression";

// Returns a list of paths with all .input files that can be found in the working_folder
std::vector<fs::path> get_test_files()
{
    std::vector<fs::path> result;

    auto range = boost::make_iterator_range(fs::directory_iterator(working_folder), {});
    for (auto& entry : range)
    {
        if (entry.path().extension() == ".input")
            result.emplace_back(entry.path());
    }

    return result;
}

// Compiles the given file_paths with the given template_name and the given options. Returns whether it succeeded
bool compile_with(std::vector<fs::path> const& file_paths, flatmessage::compiler_options const& options)
{
    flatmessage::compiler compiler;
    return compiler.compile_files(file_paths, options);
}

// Returns whether the given file_name's content matches its .expected counter part
bool test_one(fs::path file_name, std::string const& extension)
{
    namespace test = boost::spirit::x3::testing;

    auto output_file_name = file_name;
    output_file_name.replace_extension(extension);

    auto output = test::load(output_file_name);
    auto expected = test::load(output_file_name.replace_extension(extension + ".expected"));

    auto result = test::compare(output, expected);

    // adapted from
    // https://github.com/boostorg/spirit/blob/db1f83544370405c3621bee0f040c905d76e3fc6/include/boost/spirit/home/x3/support/utility/testing.hpp#L234
    if (!result.full_match)
    {
        std::cout << "=============================================\n";
        std::cout << "==== Mismatch Found:\n";
        int line = 1;
        int col = 1;
        for (auto i = output.begin(); i != result.pos; ++i)
        {
            if (*i == '\n')
            {
                line++;
                col = 0;
            }
            ++col;
        }

        std::cerr << "==== File: " << file_name.replace_extension(extension) << ", Line: " << line
                  << ", Column: " << col << "\n";
        std::cerr << "=============================================\n";

        // Print output
        std::cerr << output;
        std::cerr << "=============================================\n";
        std::cerr << "==== End\n";
        std::cerr << "=============================================\n";
        return false;
    }

    return true;
}

// Ensures that the content of the compiled files of the given file_paths matches the expected content
bool test_output(std::vector<fs::path> const& file_paths, std::vector<std::string> const& file_extensions)
{
    for (auto& path : file_paths)
    {
        for (auto& extension : file_extensions)
        {
            if (!test_one(working_folder / path.stem(), extension))
                return false;
        }
    }

    return true;
}

// Compiling multiple files with different templates should generate them accordingly
DEF_TEST(compile_many, compiler)
{
    using cf = flatmessage::compiler_flags;

    auto files = get_test_files();
    EXPECT(compile_with(files, {working_folder / "cpp.template", 1, working_folder, "cpp", cf::none}));
    EXPECT(compile_with(files, {working_folder / "hpp.template", 1, working_folder, "hpp", cf::none}));

    EXPECT(test_output(files, {"cpp", "hpp"}));

    return true;
}

// Compiling multiple files with merge flag set should generate only one big file
DEF_TEST(compiler_merge_translation_units, compiler)
{
    using cf = flatmessage::compiler_flags;

    auto files = get_test_files();
    EXPECT(compile_with(files,
                        {working_folder / "merged.template", 1, working_folder, "dmp", cf::merge_translation_units}));

    EXPECT(test_one(*files.begin(), "dmp"));

    return true;
}

// Compiling with included files from different directories should generate only our files
DEF_TEST(compiler_include_directories, compiler)
{
    using cf = flatmessage::compiler_flags;

    fs::path include_dir = working_folder / "include_test/nested";
    fs::path root_file = working_folder / "include_test/Root.input";

    EXPECT(compile_with(
        {root_file},
        {working_folder / "hpp.template", 1, working_folder / "include_test", "hpp", cf::none, {include_dir}}));

    EXPECT(test_one(root_file, "hpp"));

    // Files from the include directory should't be build
    namespace test = boost::spirit::x3::testing;
    EXPECT(test::load(working_folder / "include_test/nested/Include.hpp").empty());
    
    return true;
}