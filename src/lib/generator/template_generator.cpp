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

#include "template_renderer.hpp"

#include <flatmessage/ast/ast.hpp>
#include <flatmessage/generator/template_generator.hpp>

#include <sol.hpp>

#include <fstream>
#include <map>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

struct template_generator_impl
{
    using result_type = void;

    template_generator_impl(std::ostream& out) : out(out)
    {
        using namespace flatmessage::ast;
        using optional_string = boost::optional<std::string>;
        using optional_int = boost::optional<int>;

        // clang-format off
        lua.new_usertype<optional_string>("optional_s",
            "isset", sol::property(&optional_string::is_initialized),
            "value", sol::property(static_cast<std::string& (optional_string::*)()>(&optional_string::get))
            );

        lua.new_usertype<optional_int>("optional_i",
            "isset", sol::property(&optional_int::is_initialized),
            "value", sol::property(static_cast<int& (optional_int::*)()>(&optional_int::get))
            );

        lua.new_usertype<enum_value>("enum_value",
            "name", sol::readonly(&enum_value::name),
            "value", sol::readonly(&enum_value::value)
            );

        lua.new_usertype<enumeration>("enumeration",
            "name", sol::readonly(&enumeration::name),
            "alignment", sol::readonly(&enumeration::alignment),
            "values", sol::readonly(&enumeration::values)
            );

        lua.new_usertype<attribute>("attribute",
            "specifier", sol::readonly(&attribute::specifier),
            "type", sol::readonly(&attribute::type),
            "arraySize", sol::readonly(&attribute::arraySize),
            "name", sol::readonly(&attribute::name)
            );

        lua.new_usertype<message>("message",
            "name", sol::readonly(&message::name),
            "attributes", sol::readonly(&message::attributes)
            );

        lua.new_usertype<flatmessage::ast::data>("data",
            "name", sol::readonly(&data::name),
            "attributes", sol::readonly(&data::attributes)
            );

        lua.new_usertype<flatmessage::ast::module_decl>("module",
            "name", sol::readonly(&module_decl::name)
            );

        lua.new_usertype<flatmessage::ast::import_decl>("import",
            "name", sol::readonly(&import_decl::name)
            );

        lua.new_usertype<flatmessage::ast::protocol_decl>("protocol",
            "name", sol::readonly(&protocol_decl::name)
            );
        // clang-format on
    }

    std::string generate(std::string const& templateCode)
    {
        lua.open_libraries();

        lua["enums"] = enums;
        lua["data"] = data;
        lua["messages"] = messages;
        lua["module"] = module;
        lua["imports"] = imports;
        lua["protocol"] = protocol;

        lua.create_named_table("includes");

        sol::table renderer = lua.script(template_renderer);
        sol::function fun = renderer["compile"];
        return fun(templateCode);
    }

    void operator()(flatmessage::ast::enumeration const& enumeration);
    void operator()(flatmessage::ast::message const& message);
    void operator()(flatmessage::ast::data const& data);
    void operator()(flatmessage::ast::module_decl const& module_decl);
    void operator()(flatmessage::ast::import_decl const& import_decl);
    void operator()(flatmessage::ast::protocol_decl const& protocol_decl);

    std::ostream& out;

    std::vector<flatmessage::ast::enumeration> enums;
    std::vector<flatmessage::ast::message> messages;
    std::vector<flatmessage::ast::data> data;
    flatmessage::ast::module_decl module;
    std::vector<flatmessage::ast::import_decl> imports;
    flatmessage::ast::protocol_decl protocol;

    sol::state lua;
};

namespace flatmessage
{
    namespace generator
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
            template_generator_impl v(out);

            for (auto const& ast_ : ast)
                boost::apply_visitor(v, ast_);

            out << v.generate(_template);

            return true;
        }
    }
}

void template_generator_impl::operator()(flatmessage::ast::enumeration const& enumeration)
{
    enums.push_back(enumeration);
}

void template_generator_impl::operator()(flatmessage::ast::message const& message)
{
    messages.push_back(message);
}

void template_generator_impl::operator()(flatmessage::ast::data const& data)
{
    this->data.push_back(data);
}

void template_generator_impl::operator()(flatmessage::ast::module_decl const& module_decl)
{
    module = module_decl;
}

void template_generator_impl::operator()(flatmessage::ast::import_decl const& import_decl)
{
    imports.push_back(import_decl);
}

void template_generator_impl::operator()(flatmessage::ast::protocol_decl const& protocol_decl)
{
    protocol = protocol_decl;
}