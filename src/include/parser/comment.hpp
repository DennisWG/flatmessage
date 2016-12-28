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

#pragma once

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/spirit/home/x3.hpp>

namespace QuickMessage
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using x3::lit;
        using x3::lexeme;
        using x3::eol;
        using x3::eoi;
        using ascii::char_;

        // Parses a C-Style single line comment.
        // Example: // this is a comment!
        auto const singleLineComment = x3::rule<class singleLineComment>() = lit("//") >> *(char_ - eol) >> (eol | eoi);

        // Parses a C-Style multi line comment.
        // Example: /* this is a comment! */
        auto const multiLineComment = x3::rule<class multiLineComment>()
            = lit("/*") >> *(char_ - char_('*')) >> lit("*/");

        // Combines both single and multi line comments for the skip parser
        auto const comment = singleLineComment | multiLineComment;
    }
}