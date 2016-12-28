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

#include <boost/spirit/home/x3.hpp>

namespace QuickMessage
{
    namespace parser
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using ascii::alpha;
        using ascii::alnum;

        // An alphabetic character followed by any number of alphanumeric characters
        // Valid example: validName123
        // Invalid example: 3invalid
        auto const name = x3::rule<class name, std::string>() = alpha >> *alnum;
    }
}