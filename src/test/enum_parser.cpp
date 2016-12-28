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

#include <parser/enum.hpp>
#include <testinator.h>

DEF_TEST(ParseSingleEnum, enum_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;

    std::string const data = R"(
enum Color : qword
{
    Blue = 1,
}
)";

    auto begin = data.begin();
    auto const end = data.end();

    QuickMessage::ast::enum_ result;
    bool success = phrase_parse(begin, end, QuickMessage::parser::enum_, space, result);

    EXPECT(result.name == "Color");
    EXPECT(result.alignment == "qword");
    EXPECT(result.values.size() == 1);
    if (result.values.size() != 1)
    {
        SKIP("result.values.size() != 1");
        return false;
    }

    auto name = result.values[0].first;
    auto value = result.values[0].second;

    EXPECT(name == "Blue");
    EXPECT(value == 1);

    return success && begin == end;
}