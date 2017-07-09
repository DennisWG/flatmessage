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

#include "expression.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

#include <map>

namespace flatmessage
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;

        template <typename Iterator> using error_handler = x3::error_handler<Iterator>;

        // tag used to get our error handler from the context
        struct error_handler_tag;

        struct error_handler_base
        {
            error_handler_base();

            template <typename Iterator, typename Exception, typename Context>
            x3::error_handler_result on_error(Iterator& first, Iterator const& last, Exception const& x,
                                              Context const& context);

            std::map<std::string, std::string> id_map;
        };

        inline error_handler_base::error_handler_base()
        {
            id_map["attribute_vector"] = "one or more attributes";
            id_map["enum_size"] = "byte, word, dword or qword";
            id_map["multiplicative_expr"] = "Expression";
            id_map["enum_value_vector"] = "one or more values";
            id_map["module_identifier"] = "identifier";
        }

        template <typename Iterator, typename Exception, typename Context>
        inline x3::error_handler_result error_handler_base::on_error(Iterator& first, Iterator const& last,
                                                                     Exception const& x, Context const& context)
        {
            std::string which = x.which();
            auto iter = id_map.find(which);
            if (iter != id_map.end())
                which = iter->second;

            std::string message = "Error! Expecting " + which + " here:";
            auto& error_handler = x3::get<error_handler_tag>(context).get();
            error_handler(x.where(), message);
            return x3::error_handler_result::fail;
        }
    }
}