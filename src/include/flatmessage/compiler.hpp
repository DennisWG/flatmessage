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

#include <boost/filesystem.hpp>
#include <vector>

namespace flatmessage
{
    // A set of flags that alter the compilation process
    enum class compiler_flags
    {
        // Just compile the normal way
        none = 0,
        // Parses the translation units as if they are in one big file
        merge_translation_units = 1,
    };

    // A set of options to configure the compiler's behaviour
    struct compiler_options
    {
        // Full path to the file describing the output
        boost::filesystem::path template_file;
        // The amount of threads used for compilation
        int num_threads = 1;
        // The path where to write the output files to
        boost::filesystem::path output_path;
        // The file extension of that the output files
        std::string file_extension;
        // A set of flags that alter the compilation process
        compiler_flags flags = compiler_flags::none;
		// A list of include directories
        std::vector<boost::filesystem::path> include_directories;
    };

    // Handles compilation of file_template_pairs
    class compiler
    {
      public:
        // Compiles the given list of file_template_pairs with the given compiler_options and returns whether or not it
        // succeeded
        bool compile_files(std::vector<boost::filesystem::path> const& files, compiler_options const& options);
    };

    inline compiler_flags operator|(compiler_flags lhs, compiler_flags rhs) noexcept
    {
        using T = std::underlying_type_t<compiler_flags>;
        return static_cast<compiler_flags>((static_cast<T>(lhs) | static_cast<T>(rhs)));
    }

    inline compiler_flags& operator|=(compiler_flags& lhs, compiler_flags rhs) noexcept
    {
        using T = std::underlying_type_t<compiler_flags>;
        lhs = static_cast<compiler_flags>((static_cast<T>(lhs) | static_cast<T>(rhs)));
        return lhs;
    }

    inline compiler_flags operator&(compiler_flags lhs, compiler_flags rhs) noexcept
    {
        using T = std::underlying_type_t<compiler_flags>;
        return static_cast<compiler_flags>((static_cast<T>(lhs) & static_cast<T>(rhs)));
    }

    inline compiler_flags& operator&=(compiler_flags& lhs, compiler_flags rhs) noexcept
    {
        using T = std::underlying_type_t<compiler_flags>;
        lhs = static_cast<compiler_flags>((static_cast<T>(lhs) & static_cast<T>(rhs)));
        return lhs;
    }

    inline compiler_flags operator^(compiler_flags lhs, compiler_flags rhs) noexcept
    {
        using T = std::underlying_type_t<compiler_flags>;
        return static_cast<compiler_flags>((static_cast<T>(lhs) ^ static_cast<T>(rhs)));
    }

    inline compiler_flags& operator^=(compiler_flags& lhs, compiler_flags rhs) noexcept
    {
        using T = std::underlying_type_t<compiler_flags>;
        lhs = static_cast<compiler_flags>((static_cast<T>(lhs) ^ static_cast<T>(rhs)));
        return lhs;
    }
}