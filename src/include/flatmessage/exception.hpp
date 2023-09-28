/*
Copyright (c) 2023 Dennis Werner Garske (DWG)

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

#include <exception>
#include <string>

namespace flatmessage
{
    class exception : public std::exception
    {
      public:
        exception(std::string const& message) : msg_{message} {}

        virtual char const* what() const noexcept { return msg_.c_str(); }

      private:
        std::string msg_;
    };
}
