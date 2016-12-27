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
#include <parser/message.hpp>

DEF_TEST(ParseSingleMessage, message_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;

    std::string const data =
        "message foo"
        "{"
        "    uint8 id;"
        "}";

    auto begin = data.begin();
    auto const end = data.end();

    playground::ast::message result;
    bool success = phrase_parse(begin, end, playground::parser::message, space, result);

    EXPECT(result.name == "foo");
    EXPECT(result.attributes.size() == 1);
    if (result.attributes.size() != 1)
    {
        SKIP("result.attributes.size() != 1");
        return false;
    }

    auto rule = std::get<0>(result.attributes[0]);
    auto type = std::get<1>(result.attributes[0]);
    auto name = std::get<2>(result.attributes[0]);

    EXPECT(rule.is_initialized() == false);
    EXPECT(type == "uint8");
    EXPECT(name == "id");

    return success && begin == end;
}

DEF_TEST(ParseSingleMessageWithRules, message_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;

    std::string const data =
        "message foo"
        "{"
        "    optional uint8 id;"
        "    repeated uint32 size;"
        "}";

    auto begin = data.begin();
    auto const end = data.end();

    playground::ast::message result;
    bool success = phrase_parse(begin, end, playground::parser::message, space, result);

    EXPECT(result.name == "foo");
    EXPECT(result.attributes.size() == 2);
    if (result.attributes.size() != 2)
    {
        SKIP("result.attributes.size() != 2");
        return false;
    }

    boost::optional<std::string> rule;
    std::string type, name;
    std::tie(rule, type, name) = result.attributes[0];

    EXPECT(rule.is_initialized());
    EXPECT(rule.get() == "optional");
    EXPECT(type == "uint8");
    EXPECT(name == "id");

    rule = std::get<0>(result.attributes[1]);
    type = std::get<1>(result.attributes[1]);
    name = std::get<2>(result.attributes[1]);

    EXPECT(rule.is_initialized());
    EXPECT(rule.get() == "repeated");
    EXPECT(type == "uint32");
    EXPECT(name == "size");

    return success && begin == end;
}

DEF_TEST(ParseMultipleMessages, message_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;

    std::string const data =
        "message foo"
        "{"
        "    repeated\nuint8 id;"
        "}"
        "message bar"
        "{\n"
        "    int8 id;"
        "}";

    auto begin = data.begin();
    auto const end = data.end();

    std::vector<playground::ast::message> result;
    bool success = phrase_parse(begin, end, +playground::parser::message, space, result);

    EXPECT(result.size() == 2);

    if (result.size() != 2)
    {
        SKIP("result.size() != 2");
        return false;
    }

    EXPECT(result[0].name == "foo");
    EXPECT(result[0].attributes.size() == 1);

    if (result[0].attributes.size() != 1)
    {
        SKIP("result[0].attributes.size() != 1");
        return false;
    }

    boost::optional<std::string> rule;
    std::string type, name;
    std::tie(rule, type, name) = result[0].attributes[0];

    EXPECT(rule.is_initialized());
    EXPECT(rule.get() == "repeated");
    EXPECT(type == "uint8");
    EXPECT(name == "id");

    EXPECT(result[1].name == "bar");
    EXPECT(result[1].attributes.size() == 1);

    if (result[1].attributes.size() != 1)
    {
        SKIP("result[1].attributes.size() != 1");
        return false;
    }

    std::tie(rule, type, name) = result[1].attributes[0];

    EXPECT(rule.is_initialized() == false);
    EXPECT(type == "int8");
    EXPECT(name == "id");

    return success && begin == end;
}