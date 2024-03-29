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

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/std_array.hpp>
#include <boost/fusion/include/std_array.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <boost/fusion/include/boost_array.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>

// clang-format off

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::annotation,
    (std::string, name)
    (boost::optional<flatmessage::ast::annotation_value_t>, value)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::data,
    (std::vector<flatmessage::ast::annotation>, annotations)
    (std::string, name)
    (std::vector<flatmessage::ast::attribute>, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::message,
    (std::vector<flatmessage::ast::annotation>, annotations)
    (std::string, name)
    (std::vector<flatmessage::ast::attribute>, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::attribute,
    (std::vector<flatmessage::ast::annotation>, annotations)
    (boost::optional<std::string>, specifier)
    (std::string, type)
    (boost::optional<int>, arraySize)
    (std::string, name)
    (boost::optional<flatmessage::ast::default_value_t>, defaultValue)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::enumeration,
    (std::vector<flatmessage::ast::annotation>, annotations)
    (std::string, name)
    (std::string, alignment)
    (std::vector<flatmessage::ast::enum_value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::enum_value,
    (std::string, name)
    (int, value)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::module_decl,
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::import_decl,
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(flatmessage::ast::protocol_decl,
    (std::string, name)
)

// clang-format on
