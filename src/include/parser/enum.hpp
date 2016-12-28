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

#include "../ast/enum.hpp"
#include "common.hpp"

namespace QuickMessage
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using x3::lit;
        using x3::lexeme;
        using x3::omit;
        using ascii::char_;
        using ascii::space;
        using x3::string;
        using x3::int_;

        // Blue = 1,
        auto const enumValue = x3::rule<class enumValue, std::pair<std::string, int>>() = name >> '=' >> int_ >> ',';

        // enum Color : word { <enumValue> }
        auto const enum_ = x3::rule<class enum_, ast::enum_>()
            = lit("enum") >> name >> ':' >> (string("byte") | string("word") | string("dword") | string("qword")) >> '{'
              >> +enumValue >> '}';

        BOOST_SPIRIT_DEFINE(enum_);
    }
}