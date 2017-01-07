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

#include "../ast/ast.hpp"
#include "../ast/ast_adopted.hpp"
#include "annotation.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "expression.hpp"

#include <boost/spirit/home/x3.hpp>

namespace flatmessage
{
    namespace parser
    {
        using x3::double_;
        using x3::char_;
        using x3::raw;
        using x3::lexeme;
        using x3::string;
        using x3::int_;
        using x3::eol;
        using x3::eoi;
        using namespace x3::ascii;

        struct attribute_class;
        struct specifier_class;
        struct enum_value_class;

        using attribute_type = x3::rule<attribute_class, ast::attribute>;
        using specifier_type = x3::rule<specifier_class, std::string>;
        using enum_value_type = x3::rule<enum_value_class, ast::enum_value>;

        data_type const data = "data";
        message_type const message = "message";
        attribute_type const attribute = "attribute";
        specifier_type const specifier = "specifier";
        enumeration_type const enumeration = "enum";
        enum_value_type const enum_value = "enum_value";

        auto const attribute_vector
            = x3::rule<struct attribute_vector_class, std::vector<ast::attribute>>("attribute_vector") = +attribute;

        auto const data_def = lit("data") > identifier > '{' > attribute_vector > '}';

        auto const message_def = lit("message") > identifier > '{' > attribute_vector > '}';

        auto const attribute_def = -specifier >> identifier >> -('[' > int_ > ']') > identifier > ';';

        auto const specifier_def = string("optional") | string("repeated");

        auto const enum_value_vector
            = x3::rule<struct enum_value_vector_class, std::vector<ast::enum_value>>("enum_value_vector") = +enum_value;

        auto const enum_size = x3::rule<struct enum_size_class, std::string>("enum_size")
            = string("byte") | string("word") | string("dword") | string("qword");

        auto const enumeration_def = lit("enum") > identifier > ':' > enum_size > '{' > enum_value_vector > '}';

        auto const number = x3::rule<struct number_def, int>("number") = int_;

        auto const enum_value_def = identifier > '=' > number > ',';

        BOOST_SPIRIT_DEFINE(data, message, attribute, specifier, enumeration, enum_value);

        struct data_class : annotation_base, error_handler_base
        {
        };
        struct message_class : annotation_base, error_handler_base
        {
        };
        struct attribute_class : annotation_base
        {
        };
        struct specifier_class : annotation_base
        {
        };
        struct enumeration_class : annotation_base, error_handler_base
        {
        };
        struct enum_value_class : annotation_base
        {
        };
    }
}

namespace flatmessage
{
    parser::data_type const& data() { return parser::data; }
    parser::message_type const& message() { return parser::message; }
    parser::enumeration_type const& enumeration() { return parser::enumeration; }
}