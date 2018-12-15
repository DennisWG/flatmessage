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

#include "generator.hpp"

namespace flatmessage
{
    namespace generator
    {
        // A code generator that uses a template to generate code
        class template_generator : public generator
        {
          public:
            // Constructs the template_generator by parsing the given template_file_path
            template_generator(std::string const& template_file_path);
            // Generates the code from the given ast and writes it into the given stream
            bool generate(std::ostream& stream, ast::ast const& ast,
                          std::unordered_set<std::string> const& exported_enums,
                          std::unordered_set<std::string> const& exported_data) override;

          private:
            std::string _template;
        };
    }
}