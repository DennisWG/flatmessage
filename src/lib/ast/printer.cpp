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

#include <flatmessage/ast/printer.hpp>

namespace flatmessage::ast
{
    struct visitor
    {
        using result_type = void;

        visitor(std::ostream& out) : out(out) {}
        void operator()(enum_value const& enumValue);
        void operator()(enumeration const& enumeration);
        void operator()(attribute const& attribute);
        void operator()(message const& message);
        void operator()(data const& data);
        void operator()(module_decl const& module_decl);
        void operator()(import_decl const& import_decl);
        void operator()(protocol_decl const& protocol_decl);

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
        out << '\n';
    }

    void printAnnotation(std::vector<annotation> const& annotations, std::ostream& out)
    {
        if (annotations.empty())
            return;

        bool first = true;
        out << '[';

        for (auto&& annotation : annotations)
        {
            if (!first)
                out << ',';

            struct visit
            {
                visit(std::ostream& out) : out(out) {}

                void operator()(int intValue) { out << intValue; }
                void operator()(double doubleValue) { out << doubleValue; }
                void operator()(std::string const& stringValue) { out << stringValue; }

                std::ostream& out;
            } v{ out };

            out << annotation.name << '=';
            boost::apply_visitor(v, annotation.value);

            first = false;
        }

        out << "] ";
    }

    void printDefaultValue(boost::optional<default_value_t> const& defaultValue, std::ostream& out)
    {
        if (!defaultValue)
            return;
        out << "=";

        struct visit
        {
            visit(std::ostream& out) : out(out) {}

            void operator()(int intValue) { out << intValue; }
            void operator()(double doubleValue) { out << doubleValue; }
            void operator()(std::string const& stringValue) { out << stringValue; }

            std::ostream& out;
        } v{out};

        boost::apply_visitor(v, *defaultValue);
    }

    void flatmessage::ast::visitor::operator()(attribute const& attribute)
    {
        out << '(';
        if (attribute.specifier)
            out << *attribute.specifier << ", ";

        out << attribute.type;
        if (attribute.arraySize)
            out << '[' << *attribute.arraySize << ']';

        out << ", ";
        printAnnotation(attribute.annotations, out);
        out << attribute.name;

        printDefaultValue(attribute.defaultValue, out);

        out << ')';
    }

    void flatmessage::ast::visitor::operator()(message const& message)
    {
        out << "message " << message.name << ' ';

        for (auto const& attribute : message.attributes)
            (*this)(attribute);

        out << '\n';
    }

    void flatmessage::ast::visitor::operator()(data const& data)
    {
        out << "data " << data.name << ' ';

        for (auto const& attribute : data.attributes)
            (*this)(attribute);

        out << '\n';
    }

    void flatmessage::ast::visitor::operator()(module_decl const& module_decl)
    {
        out << "module " << module_decl.name << '\n';
    }

    void flatmessage::ast::visitor::operator()(import_decl const& import_decl)
    {
        out << "import " << import_decl.name << '\n';
    }

    void flatmessage::ast::visitor::operator()(protocol_decl const& protocol_decl)
    {
        out << "protocol " << protocol_decl.name << '\n';
    }

    void print(std::ostream& out, ast const& ast)
    {
        visitor v(out);
        for (auto const& ast_ : ast)
            boost::apply_visitor(v, ast_);
    }
}
