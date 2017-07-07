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

#include <testinator.h>

// TODO: remove this hack once boost no longer uses this deprecated function
namespace std
{
    template <class arg, class result> struct unary_function
    {
        typedef arg argument_type;
        typedef result result_type;
    };
}

#include <boost/range/iterator_range.hpp>
#include <boost/spirit/home/x3/support/utility/testing.hpp>

#include <flatmessage/compiler.hpp>

namespace fs = boost::filesystem;

using input_expect_pair = std::pair<fs::path, std::string>;

const fs::path working_folder = "../compiler_expression";

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
bool compile_with(std::vector<fs::path> const& file_paths, std::string const& template_name,
                  flatmessage::compiler_options const& options)
{
    std::vector<flatmessage::file_template_pair> compiler_input;
    for (auto& path : file_paths)
    {
        auto directory = path.parent_path();
        compiler_input.push_back({path, directory / template_name});
    }

    flatmessage::compiler compiler;
    return compiler.compile_files(compiler_input, options);
}

// Returns whether the given file_name's content matches its .expected counter part
bool test_one(fs::path file_name, std::string const& extension)
{
    namespace test = boost::spirit::x3::testing;

    auto output_file_name = file_name;
    output_file_name.replace_extension(extension);

    auto output = test::load(working_folder / output_file_name);
    auto expected = test::load(working_folder / output_file_name.replace_extension(extension + ".expected"));

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
bool test_output(std::vector<fs::path> const& file_paths)
{
    for (auto& path : file_paths)
    {
        if (!test_one(path.stem(), "cpp"))
            return false;
        if (!test_one(path.stem(), "hpp"))
            return false;
    }

    return true;
}

DEF_TEST(compile_many, compiler)
{
    auto files = get_test_files();
    EXPECT(compile_with(files, "cpp.template", {1, working_folder, "cpp"}));
    EXPECT(compile_with(files, "hpp.template", {1, working_folder, "hpp"}));

    EXPECT(test_output(files));

    return true;
}
