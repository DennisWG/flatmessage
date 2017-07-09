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

#include <flatmessage/compiler.hpp>
#include <flatmessage/parser.hpp>
#include <flatmessage/generator/template_generator.hpp>

#include <fmt/format.h>

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace flatmessage
{
    namespace fs = boost::filesystem;

    // Representation of a translation unit
    struct translation_unit
    {
        // Where is the file stored and what is it called
        fs::path file_path;
        // Where is the template file stored that goes with this translation unit
        fs::path template_path;
        // The AST representing the content
        flatmessage::ast::ast ast;
        // The module this translation unit is a part of
        std::string module;
        // A list of names of the modules that this translation unit imports
        std::vector<std::string> imported_modules;
        // The protocol this translation unit represents
        std::string protocol;
        // A list of 'data' structures that the translatio unit exports when imported
        std::vector<std::string> exported_types;
        // A list of 'data' structures that the translatio unit imports from other modules
        std::vector<std::string> imported_types;

        translation_unit() = default;
        translation_unit(flatmessage::ast::ast const& ast) : ast(ast)
        {
            struct visitor
            {
                using result_type = void;

                visitor()
                {
                    buildin_types.insert("byte");
                    buildin_types.insert("uint8");
                    buildin_types.insert("int8");
                    buildin_types.insert("uint16");
                    buildin_types.insert("int16");
                    buildin_types.insert("uint32");
                    buildin_types.insert("int32");
                    buildin_types.insert("uint64");
                    buildin_types.insert("int64");
                    buildin_types.insert("float");
                    buildin_types.insert("string");
                }

                void operator()(flatmessage::ast::enumeration const& enumeration) {}
                void operator()(flatmessage::ast::attribute const& attribute)
                {
                    if (buildin_types.find(attribute.type) != buildin_types.end())
                        return;

                    if (found_data_types.find(attribute.type) != found_data_types.end())
                        return;

                    found_data_types.insert(attribute.type);
                    imported_types.push_back(attribute.type);
                }
                void operator()(flatmessage::ast::message const& message)
                {
                    for (auto const& attribute : message.attributes)
                        (*this)(attribute);
                }
                void operator()(flatmessage::ast::data const& data)
                {
                    exported_types.push_back(data.name);

                    for (auto const& attribute : data.attributes)
                        (*this)(attribute);
                }
                void operator()(flatmessage::ast::module_decl const& module_decl) { module = module_decl.name; }
                void operator()(flatmessage::ast::import_decl const& import_decl)
                {
                    imported_modules.push_back(import_decl.name);
                }
                void operator()(flatmessage::ast::protocol_decl const& protocol_decl) { protocol = protocol_decl.name; }

                std::unordered_set<std::string> buildin_types, found_data_types;
                std::string module, protocol;
                std::vector<std::string> imported_modules, exported_types, imported_types;
            } v;

            for (auto& elem : ast)
                boost::apply_visitor(v, elem);

            module = std::move(v.module);
            protocol = std::move(v.protocol);
            imported_modules = std::move(v.imported_modules);
            exported_types = std::move(v.exported_types);
            imported_types = std::move(v.imported_types);
        }
    };

    class compiler_impl
    {
        // Returns true if the given translation_unit's module name hasn't been encountered yet or false
        bool ensure_unique_module_name(translation_unit const& translation_unit)
        {
            if (_modules.find(translation_unit.module) != _modules.end())
                return false;

            _modules[translation_unit.module] = translation_unit;
            return true;
        }

        // Returns empty string if the given translation_unit's imported modules can be found or the name of the module
        // that couldn't be found
        std::string ensure_imports_exist(translation_unit const& translation_unit) const
        {
            for (auto& module : translation_unit.imported_modules)
            {
                if (_modules.find(module) == _modules.end())
                    return module;
            }

            return {};
        }

        // Returns empty string if the imported data types from the given translation_unit can be found. Returns name of
        // first missing data type otherwhise
        std::string ensure_types_exist(translation_unit const& tu,
                                       std::unordered_set<std::string> const& exported_types)
        {
            for (auto& data_type : tu.imported_types)
            {
                if (exported_types.find(data_type) == exported_types.end())
                    return data_type;
            }

            return "";
        }

        // Displays the given error_message and some information about the given translation_unit to the standard output
        void error(translation_unit const& translation_unit, std::string const& error_message)
        {
            std::cerr << "Error while compiling file '" << translation_unit.file_path.string() << "':\n"
                      << error_message << "\n";
        }

        // Stores translation_units by their module names
        std::unordered_map<std::string, translation_unit> _modules;

      public:
        // Parses the given list of files using the given amount of threads and returns the list of parsed translation
        // units
        // TODO: implement threading
        std::vector<translation_unit> parse_files(std::vector<flatmessage::file_template_pair> const& files,
                                                  int num_threads)
        {
            std::vector<translation_unit> translation_units;

            // TODO: implement threading
            for (auto& entry : files)
            {
                std::string error_message;
                auto ast = parser::parse_file(entry.input_file, error_message);
                translation_unit tu{*ast};
                tu.file_path = entry.input_file;
                tu.template_path = entry.template_file;

                translation_units.emplace_back(std::move(tu));
            }

            return translation_units;
        }

        // Analyzes the semantics of the given translation_units and returns whether it succeeded
        bool semantic_analyze(std::vector<translation_unit> const& translation_units)
        {
            _modules.clear();
            std::unordered_set<std::string> exported_types;
            // First pass - this->_modules hasn't been populated yet
            for (auto& translation_unit : translation_units)
            {
                if (!ensure_unique_module_name(translation_unit))
                {
                    error(translation_unit,
                          fmt::format("Module name '{0}' must be unique! It was already definied in {1}",
                                      translation_unit.module, _modules[translation_unit.module].file_path.string()));
                    return false;
                }

                exported_types.insert(translation_unit.exported_types.begin(), translation_unit.exported_types.end());
            }

            // Second pass - this->_modules has been populated
            for (auto& translation_unit : translation_units)
            {
                // TODO: Remove scope once 'Selection statements with initializer' are available in Visual Studio
                // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r1.html
                {
                    auto name = ensure_imports_exist(translation_unit);
                    if (!name.empty())
                    {
                        error(translation_unit, fmt::format("Imported module '{0}' couldn't be found", name));
                        return false;
                    }
                }
                {
                    auto name = ensure_types_exist(translation_unit, exported_types);
                    if (!name.empty())
                    {
                        error(translation_unit, fmt::format("Undefined data type '{0}'", name));
                        return false;
                    }
                }
            }

            return true;
        }

        // Generates the code
        void generate_code(std::vector<translation_unit> const& translation_units, fs::path const& output_path,
                           std::string const& file_extension)
        {
            // TODO: implement threading
            for (auto& translation_unit : translation_units)
            {
                auto file_name = translation_unit.file_path.stem();
                boost::filesystem::path out_file_path
                    = fmt::format("{0}/{1}.{2}", output_path.string(), file_name.string(), file_extension);

                if (!boost::filesystem::exists(out_file_path.parent_path()))
                    boost::filesystem::create_directory(out_file_path.parent_path());

                std::ofstream out_file(out_file_path.c_str());

                flatmessage::generator::template_generator generator(translation_unit.template_path.string());
                generator.generate(out_file, translation_unit.ast);
            }
        }
    };

    bool compiler::compile_files(std::vector<file_template_pair> const& files, compiler_options const& options)
    {
        compiler_impl impl;

        auto translation_units = impl.parse_files(files, options.num_threads);

        if (!impl.semantic_analyze(translation_units))
            return false;

        impl.generate_code(translation_units, options.output_path, options.file_extension);
        return true;
    }
}