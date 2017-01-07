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

#include <ast/ast.hpp>
#include <generator/cpp_generator.hpp>

#include <map>

struct generator
{
    using result_type = void;

    generator(std::ostream& out) : out(out)
    {
        type_aliases["byte"] = "uint8_t";
        type_aliases["uint8"] = "uint8_t";
        type_aliases["int8"] = "int8_t";
        type_aliases["word"] = "uint16_t";
        type_aliases["uint16"] = "uint16_t";
        type_aliases["int16"] = "int16_t";
        type_aliases["dword"] = "uint32_t";
        type_aliases["uint32"] = "uint32_t";
        type_aliases["int32"] = "int32_t";
        type_aliases["qword"] = "uint64_t";
        type_aliases["uint64"] = "uint64_t";
        type_aliases["int64"] = "int64_t";
    }

    void operator()(flatmessage::ast::enum_value const& enumValue);
    void operator()(flatmessage::ast::enumeration const& enumeration);
    void operator()(flatmessage::ast::attribute const& attribute);
    void operator()(flatmessage::ast::message const& message);
    void operator()(flatmessage::ast::data const& data);

    std::ostream& out;

    std::map<std::string, std::string> type_aliases;
};

namespace flatmessage
{
    namespace generator
    {
        bool cpp_generator::generate(std::ostream& out, flatmessage::ast::ast const& ast)
        {
            ::generator v(out);

            for (auto const& ast_ : ast)
                boost::apply_visitor(v, ast_);

            return true;
        }
    }
}

void generator::operator()(flatmessage::ast::enum_value const& enumValue)
{
    out << "    ";

    out << enumValue.name << " = ";
    out << enumValue.value << ',';

    out << std::endl;
}

void generator::operator()(flatmessage::ast::enumeration const& enumeration)
{
    out << "// Generated Code. Do not edit!" << std::endl;
    out << "enum class " << enumeration.name << " : " << type_aliases[enumeration.alignment] << std::endl;
    out << '{' << std::endl;

    for (auto const& value : enumeration.values)
        (*this)(value);

    out << '}' << std::endl << std::endl;
}

void generator::operator()(flatmessage::ast::attribute const& attribute)
{
    out << "    ";

    auto writeType = [&]() {
        auto itr = type_aliases.find(attribute.type);
        if (itr != type_aliases.end())
            out << itr->second;
        else
            out << attribute.type;
    };

    auto writeName = [&]() { out << ' ' << attribute.name; };

    auto writeArray = [&]() {
        out << "std::array<";
        writeType();
        out << ", " << *attribute.arraySize << '>';
    };

    auto writeSpecifier = [&](std::string const& type) {
        out << type << '<';

        if (attribute.arraySize)
            writeArray();
        else
            writeType();

        out << '>';
        writeName();
    };

    if (attribute.specifier)
    {
        if (*attribute.specifier == "repeated")
            writeSpecifier("std::vector");
        else if (*attribute.specifier == "optional")
            writeSpecifier("boost::optional");
        else
            throw std::exception("Invalid specifier");
    }
    else
    {
        writeType();
        writeName();
    }

    out << ";" << std::endl;
}

void generator::operator()(flatmessage::ast::message const& message)
{
    out << "// Generated Code. Do not edit!" << std::endl;
    out << "struct " << message.name << " : public flatmessage::message" << std::endl;
    out << '{' << std::endl;

    for (auto const& attribute : message.attributes)
        (*this)(attribute);

    out << '}' << std::endl << std::endl;
}

void generator::operator()(flatmessage::ast::data const& data)
{
    out << "// Generated Code. Do not edit!" << std::endl;
    out << "struct " << data.name << std::endl;
    out << '{' << std::endl;

    for (auto const& attribute : data.attributes)
        (*this)(attribute);

    out << '}' << std::endl << std::endl;
}