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

#include "../ast/ast.hpp"
#include "config.hpp"

#include <boost/spirit/home/x3.hpp>

namespace flatmessage
{
    namespace x3 = boost::spirit::x3;

    namespace parser
    {
        struct data_class;
        struct message_class;
        struct enumeration_class;

        using data_type = x3::rule<data_class, ast::data>;
        using message_type = x3::rule<message_class, ast::message>;
        using enumeration_type = x3::rule<enumeration_class, ast::enumeration>;

        BOOST_SPIRIT_DECLARE(data_type, message_type, enumeration_type);
    }

    parser::data_type const& data();
    parser::message_type const& message();
    parser::enumeration_type const& enumeration();
}