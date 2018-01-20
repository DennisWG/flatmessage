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

#include "../ast/ast_adopted.hpp"
#include "annotation.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "expression.hpp"
#include <flatmessage/ast/ast.hpp>

#include <boost/spirit/home/x3.hpp>

namespace flatmessage
{
    namespace parser
    {
        using x3::char_;
        using x3::double_;
        using x3::eoi;
        using x3::eol;
        using x3::int_;
        using x3::lexeme;
        using x3::raw;
        using x3::string;
        using namespace x3::ascii;

        struct attribute_class;
        struct default_value_class;
        struct specifier_class;
        struct enum_value_class;

        using attribute_type = x3::rule<attribute_class, ast::attribute>;
        using default_value_type = x3::rule<default_value_class, ast::default_value_t>;
        using specifier_type = x3::rule<specifier_class, std::string>;
        using enum_value_type = x3::rule<enum_value_class, ast::enum_value>;

        data_type const data = "data";
        message_type const message = "message";
        attribute_type const attribute = "attribute";
        default_value_type const default_value = "default_value";
        specifier_type const specifier = "specifier";
        enumeration_type const enumeration = "enum";
        enum_value_type const enum_value = "enum_value";
        module_decl_type const module_decl = "module";
        import_decl_type const import_decl = "import";
        protocol_decl_type const protocol_decl = "protocol";

        auto const attribute_vector
            = x3::rule<struct attribute_vector_class, std::vector<ast::attribute>>("attribute_vector") = +attribute;

        auto const data_def = lit("data") > identifier > '{' > attribute_vector > '}';

        auto const message_def = lit("message") > identifier > '{' > attribute_vector > '}';

        auto const attribute_def = -specifier >> identifier >> -('[' > int_ > ']') > identifier
                                   > -(default_value) > ';';

        auto const quoted_string = x3::rule<struct quoted_string_class, std::string>("quoted_string") = lexeme['"' >> +(char_ - '"') >> '"'];

        auto const float_def = x3::rule<struct float_def_class, double>("float") = int_ >> (char_('.') > int_) >> char_('f');

        auto const double_def = x3::rule<struct double_def_class, double>("double") = int_ >> (char_('.') > int_);

        auto const default_value_def = '=' > (quoted_string | float_def | double_def | int_);

        // TODO: unused
        auto const default_array_value_def = '{' > (default_value % ',') > '}';

        auto const specifier_def = string("optional") | string("repeated");

        auto const enum_value_vector
            = x3::rule<struct enum_value_vector_class, std::vector<ast::enum_value>>("enum_value_vector") = +enum_value;

        auto const enum_size = x3::rule<struct enum_size_class, std::string>("enum_size")
            = string("byte") | string("word") | string("dword") | string("qword");

        auto const enumeration_def = lit("enum") > identifier > ':' > enum_size > '{' > enum_value_vector > '}';

        auto const number = x3::rule<struct number_def, int>("number") = int_;

        auto const enum_value_def = identifier > '=' > number > ',';

        auto const module_identifier = x3::rule<struct module_identifier_class, std::string>("module_identifier")
            = raw[lexeme[(alpha | '_') >> *(alnum | '_' | '.')]];

        auto const module_decl_def = lit("module") > module_identifier > ';';

        auto const import_decl_def = lit("import") > module_identifier > ';';

        auto const protocol_decl_def = lit("protocol") > identifier > ';';

        BOOST_SPIRIT_DEFINE(data, message, default_value, attribute, specifier,
                            enumeration, enum_value, module_decl, import_decl, protocol_decl);

        struct data_class : annotation_base, error_handler_base
        {
        };
        struct message_class : annotation_base, error_handler_base
        {
        };
        struct default_value_class : annotation_base
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
        struct module_decl_class : annotation_base, error_handler_base
        {
        };
        struct import_decl_class : annotation_base, error_handler_base
        {
        };
        struct protocol_decl_class : annotation_base, error_handler_base
        {
        };
    }
}

namespace flatmessage
{
    parser::data_type const& data() { return parser::data; }
    parser::message_type const& message() { return parser::message; }
    parser::enumeration_type const& enumeration() { return parser::enumeration; }
    parser::module_decl_type const& module_decl() { return parser::module_decl; }
    parser::import_decl_type const& import_decl() { return parser::import_decl; }
    parser::protocol_decl_type const& protocol_decl() { return parser::protocol_decl; }
}