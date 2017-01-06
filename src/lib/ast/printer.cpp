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

#include <ast/printer.hpp>

namespace flatmessage
{
    namespace ast
    {
        struct visitor
        {
            using result_type = void;

            visitor(std::ostream& out) : out(out) {}
            void operator()(enum_value const& enumValue);
            void operator()(enumeration const& enumeration);
            void operator()(attribute const& attribute);
            void operator()(message const& message);

            std::ostream& out;
        };

        void flatmessage::ast::visitor::operator()(enum_value const& enumValue)
        {
            out << '(';

            out << enumValue.name << ", ";
            out << enumValue.value;

            out << ')';
        }

        void flatmessage::ast::visitor::operator()(enumeration const& enumeration)
        {
            out << "enum " << enumeration.name << ':' << enumeration.alignment << ' ';

            for (auto const& value : enumeration.values)
                (*this)(value);
            out << std::endl;
        }

        void flatmessage::ast::visitor::operator()(attribute const& attribute)
        {
            out << '(';
            if (attribute.specifier)
                out << *attribute.specifier << ", ";

            out << attribute.type << ", ";
            out << attribute.name;
            out << ')';
        }

        void flatmessage::ast::visitor::operator()(message const& message)
        {
            out << "message " << message.name << ' ';

            for (auto const& attribute : message.attributes)
                (*this)(attribute);

            out << std::endl;
        }

        void print(std::ostream& out, ast const& ast)
        {
            visitor v(out);
            for (auto const& ast_ : ast)
                boost::apply_visitor(v, ast_);
        }
    }
}