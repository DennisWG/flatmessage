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

#include <flatmessage/ast/ast.hpp>

#include <optional>

namespace boost::filesystem
{
    class path;
}

namespace flatmessage
{
    namespace parser
    {
        // Parses the given string and returns an AST representing its content if parsing succeeded. If it failed the
        // given out_error will contain the failure reason. Can also be given an optional source which will be displayed
        // within the error message
        std::optional<ast::ast> parse_string(std::string const& content, std::string& out_error,
                                             std::string const& source = "");

        // Parses the file at the given file_path and returns an AST representing its content if it succeeded. If it
        // failed the given out_error will contain the failure reason
        std::optional<ast::ast> parse_file(boost::filesystem::path const& file_path, std::string& out_error);
    }
}