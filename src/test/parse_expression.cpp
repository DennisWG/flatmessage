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

#include <flatmessage/ast/printer.hpp>
#include <flatmessage/parser.hpp>

#include <testinator.h>

#include <boost/variant.hpp>

namespace fs = boost::filesystem;
namespace testing = boost::spirit::x3::testing;

auto parse = [](std::string const& source, fs::path inputPath) {
    std::stringstream out;
    std::string error_message;

    if (auto ast = flatmessage::parser::parse_string(source, error_message, inputPath.string()))
    {
        flatmessage::ast::print(out, *ast);
        return out.str();
    }

    return error_message;
};

DEF_TEST(ParseInputFiles, parse_expression)
{
    auto compare
        = [](fs::path inputPath, fs::path expectPath) { return testing::compare(inputPath, expectPath, parse); };

    auto path = fs::current_path() / "parse_expression";

    bool success = true;

    int successful = 0, errors = 0;
    for (auto i = fs::directory_iterator(path); i != fs::directory_iterator(); ++i)
    {
        auto ext = fs::extension(i->path());
        if (ext == ".input")
        {
            auto input_path = i->path();
            auto expect_path = input_path;
            expect_path.replace_extension(".expect");
            if (!compare(input_path, expect_path))
            {
                ++errors;
                success = false;
            }
            else
            {
                ++successful;
            }
        }
    }

    std::cout << std::format("Parsed {} templates. Successful: {}, Errors: {}", successful + errors, successful, errors)
              << std::endl;

    return success;
}