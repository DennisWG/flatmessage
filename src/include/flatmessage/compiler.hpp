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
    // Stores the path to input files and their corresponding template files used by the compiler
    struct file_template_pair
    {
        // Full path to the file describing the content
        boost::filesystem::path input_file;
        // Full path to the file describing the output
        boost::filesystem::path template_file;
    };

    // A set of options to configure the compiler's behaviour
    struct compiler_options
    {
        // The amount of threads used for compilation
        int num_threads = 1;
        // The path where to write the output files to
        boost::filesystem::path output_path;
        // The file extension of that the output files
        std::string file_extension;
    };

    // Handles compilation of file_template_pairs
    class compiler
    {
      public:
        // Compiles the given list of file_template_pairs with the given compiler_options and returns whether or not it
        // succeeded
        bool compile_files(std::vector<file_template_pair> const& files, compiler_options const& options);
    };
}