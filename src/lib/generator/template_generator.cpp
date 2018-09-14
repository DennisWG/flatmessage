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

#include <flatmessage/ast/ast.hpp>
#include <flatmessage/generator/template_generator.hpp>

// clang-format off
#include <nlohmann/json.hpp>
#include <inja.hpp>
// clang-format on

#include <fstream>
#include <map>
#include <variant>
#include <optional>

using nlohmann::json;

struct template_generator_impl
{
    using result_type = void;

    std::string generate(std::string const& templateCode);

    void operator()(flatmessage::ast::enumeration const& enumeration);
    void operator()(flatmessage::ast::message const& message);
    void operator()(flatmessage::ast::data const& data);
    void operator()(flatmessage::ast::module_decl const& module_decl);
    void operator()(flatmessage::ast::import_decl const& import_decl);
    void operator()(flatmessage::ast::protocol_decl const& protocol_decl);

    nlohmann::json ast;

    std::vector<flatmessage::ast::enumeration> enums;
    std::vector<flatmessage::ast::message> messages;
    std::vector<flatmessage::ast::data> data;
    flatmessage::ast::module_decl module;
    std::vector<flatmessage::ast::import_decl> imports;
    flatmessage::ast::protocol_decl protocol;
};

namespace flatmessage::generator
{
    template_generator::template_generator(std::string const& template_file_path)
    {
        std::ifstream file(template_file_path);
        if (!file)
            throw std::exception{("Invalid file path \"" + template_file_path + "\"").c_str()};

        _template = std::string{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
    }

    bool template_generator::generate(std::ostream& out, flatmessage::ast::ast const& ast)
    {
        template_generator_impl v;

        for (auto const& ast_ : ast)
            boost::apply_visitor(v, ast_);

        out << v.generate(_template);

        return true;
    }
}

template <typename F> auto doWithAnnotation(json annotations, std::string const& name, F&& f)
{
    for (auto&& it : annotations)
    {
        if (it["name"] == name)
            return f(it);
    }

    return decltype(f({})){};
}

json getAnnotations(std::vector<flatmessage::ast::annotation> const& annotations)
{
    json annos;
    for (auto&& annotation : annotations)
    {
        type_visitor v;
        if (annotation.value)
            boost::apply_visitor(v, *annotation.value);

        annos.emplace_back(json{{"name", annotation.name}, {"value", v.myValue}});
    }

    return annos;
}

std::string template_generator_impl::generate(std::string const& templateCode)
{
    ast["hasEnums"] = !ast["enums"].empty();
    ast["hasData"] = !ast["data"].empty();
    ast["hasMessages"] = !ast["messages"].empty();
    ast["hasImports"] = !ast["imports"].empty();

    inja::Environment env;

    env.add_callback("hasAnnotation", 2, [&env](inja::Parsed::Arguments args, json data) {
        auto object = env.get_argument<json>(args, 0, data);
        auto annotation = env.get_argument<std::string>(args, 1, data);

        if (object["annotations"].empty())
            return false;

        auto annotations = object["annotations"];
        return doWithAnnotation(annotations, annotation, [&](json const& it) { return true; });
    });

    
    env.add_callback("annotationValue", 2, [&env](inja::Parsed::Arguments args, json data) {
        auto object = env.get_argument<json>(args, 0, data);
        auto annotation = env.get_argument<std::string>(args, 1, data);

        if (object["annotations"].empty())
            return json{};

        auto annotations = object["annotations"];
        return doWithAnnotation(annotations, annotation, [&](json const& it) { return it["value"]; });
    });
    

    return env.render(templateCode, ast);
}

void template_generator_impl::operator()(flatmessage::ast::enumeration const& enumeration)
{
    json values;

    // clang-format off
    for (auto&& value : enumeration.values)
    {
        values.push_back({
            {"name", value.name},
            {"value", value.value}
        });
    }

    json obj {
        {"name", enumeration.name},
        {"alignment",enumeration.alignment},
        {"values", values},
        {"hasAnnotations", !enumeration.annotations.empty()},
        {"annotations", getAnnotations (enumeration.annotations)},
    };
    // clang-format on

    ast["enums"].push_back(obj);
}
struct type_visitor
{
    void operator()(int intValue) { myValue = intValue; }
    void operator()(double doubleValue) { myValue = doubleValue; }
    void operator()(std::string const& stringValue) { myValue = stringValue; }

    json myValue;
};

std::string toMysqlType(std::string const& type)
{
    std::map<std::string, std::string> typeMap{
        {"uint8", "TINYINT UNSIGNED"},
        {"byte", "TINYINT UNSIGNED"},
        {"uint16", "SMALLINT UNSIGNED"},
        {"uint32", "INT UNSIGNED"},
        {"uint64", "BIGINT UNSIGNED"},
        {"int8", "TINYINT"},
        {"int16", "SMALLINT"},
        {"int32", "INT"},
        {"int64", "BIGINT"},
        {"char", "CHAR(1)"},
        {"float", "FLOAT"},
        {"string", "TEXT"},
    };

    if (auto itr = typeMap.find(type); itr != typeMap.end())
        return itr->second;

    return {};
}

json convertAttributes(std::vector<flatmessage::ast::attribute> const& attributes)
{
    json attribs;
    for (auto&& attrib : attributes)
    {
        json specifier;
        if (attrib.specifier)
            specifier = *attrib.specifier;

        json arraySize;
        if (attrib.arraySize)
            arraySize = *attrib.arraySize;

        type_visitor v;
        if (attrib.defaultValue)
            boost::apply_visitor(v, *attrib.defaultValue);

        // clang-format off
        attribs.push_back({
            {"hasSpecifier", !specifier.empty()},
            {"specifier", specifier},
            {"type", attrib.type},
            {"hasArraySize", !arraySize.empty()},
            {"arraySize", arraySize},
            {"name", attrib.name},
            {"hasDefaultValue", !v.myValue.empty()},
            {"defaultValue", v.myValue},
            {"hasAnnotations", !attrib.annotations.empty()},
            {"annotations", getAnnotations (attrib.annotations)},
            {"mysqlType", toMysqlType(attrib.type) },
        });
        // clang-format on
    }

    return attribs;
}

void template_generator_impl::operator()(flatmessage::ast::message const& message)
{
    // clang-format off
    json obj {
        {"name", message.name},
        {"attributes", convertAttributes(message.attributes)},
        {"hasAnnotations", !message.annotations.empty()},
        {"annotations", getAnnotations (message.annotations)},
    };
    // clang-format on

    ast["messages"].push_back(obj);
}

void template_generator_impl::operator()(flatmessage::ast::data const& data)
{
    // clang-format off
    json obj {
        {"name", data.name},
        {"attributes", convertAttributes(data.attributes)},
        {"hasAnnotations", !data.annotations.empty()},
        {"annotations", getAnnotations (data.annotations)},
    };
    // clang-format on

    ast["data"].push_back(obj);
}

auto explode(std::string const& str, char delim = ' ')
{
    std::vector<std::string> result;
    std::istringstream ss(str);

    for (std::string token; std::getline(ss, token, delim);)
        result.push_back(std::move(token));

    return result;
}

void template_generator_impl::operator()(flatmessage::ast::module_decl const& module_decl)
{
    ast["fullModule"] = module_decl.name;
    auto modulePath = explode(module_decl.name, '.');

    ast["moduleName"] = modulePath.back();
    ast["modulePath"] = std::vector<std::string>{modulePath.begin(), modulePath.end() - 1};
}

void template_generator_impl::operator()(flatmessage::ast::import_decl const& import_decl)
{
    auto importPath = explode(import_decl.name, '.');

    // clang-format off
    json import{
        {"fullImport", import_decl.name},
        {"importName", importPath.back()},
        {"importPath", std::vector<std::string>{ importPath.begin(), importPath.end() - 1 } }
    };
    // clang-format on
    ast["imports"].push_back(import);
}

void template_generator_impl::operator()(flatmessage::ast::protocol_decl const& protocol_decl)
{
    ast["protocol"] = protocol_decl.name;
}
