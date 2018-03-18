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

#include <flatmessage/ast/printer.hpp>
#include <flatmessage/generator/template_generator.hpp>
#include <flatmessage/parser.hpp>

#include <boost/spirit/home/x3/support/utility/testing.hpp>

namespace fs = boost::filesystem;
namespace testing = boost::spirit::x3::testing;

auto template_generate = [](std::string const& source, fs::path inputPath) {
    auto path = inputPath.parent_path();
    auto fileName = inputPath.stem().string() + ".template";

    std::string error_message;
    auto ast = flatmessage::parser::parse_string(source, error_message, inputPath.string());
    if (ast)
    {
        flatmessage::generator::template_generator generator((path / fileName).string());
        std::stringstream out;

        generator.generate(out, *ast);
        return out.str();
    }

    return error_message;
};

DEF_TEST(GenerateInputFiles, template_generator)
{
    auto compare = [](fs::path inputPath, fs::path expectPath) {
        return testing::compare(inputPath, expectPath, template_generate);
    };

    auto path = fs::current_path() / "generate_expression";

    bool success = true;

    for (auto i = fs::directory_iterator(path); i != fs::directory_iterator(); ++i)
    {
        auto ext = fs::extension(i->path());
        if (ext == ".input")
        {
            auto input_path = i->path();
            auto expect_path = input_path;
            expect_path.replace_extension(".expect");
            if (!compare(input_path, expect_path))
                success = false;
        }
    }

    return true;
}