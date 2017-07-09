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

#include "error_handler.hpp"

#include <boost/spirit/home/x3.hpp>

namespace flatmessage
{
    namespace parser
    {
        // Our Iterator Type
        using iterator_type = std::string::const_iterator;

        // The Phrase Parse Context
        using phrase_context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;

        // Our Error Handler
        using error_handler_type = error_handler<iterator_type>;

        // Combined Error Handler and Phrase Parse Context
        using context_type = x3::with_context<error_handler_tag, std::reference_wrapper<error_handler_type> const,
                                              phrase_context_type>::type;
    }
}