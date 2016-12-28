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

#include <parser/comment.hpp>
#include <parser/message.hpp>
#include <testinator.h>

DEF_TEST(ParseSingleLineComment, comment_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;
    using QuickMessage::parser::singleLineComment;

    std::string const data = "//message foo"
                             "message foo"
                             "{\n"
                             "message bar {"
                             "    uint8 id;"
                             "}";

    auto begin = data.begin();
    auto const end = data.end();
    auto skipper = space | singleLineComment;

    QuickMessage::ast::message result;
    bool success = phrase_parse(begin, end, QuickMessage::parser::message, skipper, result);

    EXPECT(result.name == "bar");

    return success && begin == end;
}

DEF_TEST(ParseMultiLineComment, comment_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;
    using QuickMessage::parser::multiLineComment;

    std::string const data = "/*message foo"
                             "message foo"
                             "{*/"
                             "message bar {"
                             "    uint8 id;"
                             "}";

    auto begin = data.begin();
    auto const end = data.end();
    auto skipper = space | multiLineComment;

    QuickMessage::ast::message result;
    bool success = phrase_parse(begin, end, QuickMessage::parser::message, skipper, result);

    EXPECT(result.name == "bar");

    return success && begin == end;
}

DEF_TEST(ParseMixedComments, comment_parser)
{
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::ascii::space;
    using QuickMessage::parser::comment;

    std::string const data =
        R"(
/*message foo"
{
    uint8 test;
*/
//message bar
message foobar
{
    uint32 wasd;
})";

    auto begin = data.begin();
    auto const end = data.end();
    auto skipper = space | comment;

    QuickMessage::ast::message result;
    bool success = phrase_parse(begin, end, QuickMessage::parser::message, skipper, result);

    EXPECT(result.name == "foobar");

    return success && begin == end;
}