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

#include <map>

namespace flatmessage
{
    namespace parser
    {
        struct annotation_base
        {
            template <typename T, typename Iterator, typename Context>
            inline void on_success(Iterator const& first, Iterator const& last, T& ast, Context const& context);
        };

        template <typename T, typename Iterator, typename Context>
        inline void annotation_base::on_success(Iterator const& first, Iterator const& last, T& ast,
                                                Context const& context)
        {
            auto& error_handler = x3::get<error_handler_tag>(context).get();
            error_handler.tag(ast, first, last);
        }
    }
}
