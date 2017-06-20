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

#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <list>
#include <vector>

namespace flatmessage
{
    namespace ast
    {
        namespace x3 = boost::spirit::x3;

        struct nil {};
        struct data;
        struct message;
        struct attribute;
        struct enumeration;
        struct enum_value;

        struct data : x3::position_tagged
        {
            std::string name;
            std::vector<attribute> attributes;
        };

        struct message : x3::position_tagged
        {
            std::string name;
            std::vector<attribute> attributes;
        };

        struct attribute : x3::position_tagged
        {
            boost::optional<std::string> specifier;
            std::string type;
            boost::optional<int> arraySize;
            std::string name;
        };

        struct enumeration : x3::position_tagged
        {
            std::string name;
            std::string alignment;
            std::vector<enum_value> values;
        };

        struct enum_value : x3::position_tagged
        {
            std::string name;
            int value;
        };

        struct module_decl : x3::position_tagged
        {
            std::string name;
        };

        struct import_decl : x3::position_tagged
        {
            std::string name;
        };

        struct protocol_decl : x3::position_tagged
        {
            std::string name;
        };

        using ast = std::vector<boost::variant<message, enumeration, data, module_decl, import_decl, protocol_decl>>;

        // print functions for debugging
        inline std::ostream& operator<<(std::ostream& out, nil)
        {
            out << "nil";
            return out;
        }
    }
}