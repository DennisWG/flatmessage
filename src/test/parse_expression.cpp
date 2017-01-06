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

#include <ast/ast.hpp>
#include <ast/printer.hpp>
#include <parser/config.hpp>
#include <parser/error_handler.hpp>
#include <parser/expression.hpp>

// TODO: remove this hack once boost no longer uses this deprecated function
namespace std
{
    template <class arg, class result> struct unary_function
    {
        typedef arg argument_type;
        typedef result result_type;
    };
}

#include <boost/spirit/home/x3/support/utility/testing.hpp>
#include <boost/variant.hpp>

namespace fs = boost::filesystem;
namespace testing = boost::spirit::x3::testing;

auto parse = [](std::string const& source, fs::path inputPath) {
    using flatmessage::parser::error_handler_type;
    using flatmessage::parser::iterator_type;
    using boost::spirit::x3::with;
    using boost::spirit::x3::ascii::space;

    iterator_type iter(source.begin());
    iterator_type end(source.end());

    std::stringstream out;
    error_handler_type errorHandler(iter, end, out, inputPath.string());

    auto const parser = with<flatmessage::parser::error_handler_tag>(
        std::ref(errorHandler))[+(flatmessage::message() | flatmessage::enumeration())];

    using result_type = std::vector<boost::variant<flatmessage::ast::message, flatmessage::ast::enumeration>>;

    result_type result;
    bool success = flatmessage::x3::phrase_parse(iter, end, parser, space, result);

    if (success)
    {
        if (iter != end)
            return "Error! Expecting end of input here: " + std::string(iter, end) + '\n';

        flatmessage::ast::print(out, result);
    }

    return out.str();
};

DEF_TEST(ParseInputFiles, parse_expression)
{
    auto compare
        = [](fs::path inputPath, fs::path expectPath) { return testing::compare(inputPath, expectPath, parse); };

    auto path = fs::current_path() / "parse_expression";

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

    return success;
}