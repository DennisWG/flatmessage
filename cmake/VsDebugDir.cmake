# Copyright (c) 2016 Dennis Werner Garske (DWG)

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Sets the debug directory of the Visual Studio's vs_project_name to the given directory
FUNCTION(SET_DEBUG_DIRECTORY vs_project_name directory)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.10.0.0)
            message(FATAL_ERROR "Insufficient msvc version")
        endif()
        file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${vs_project_name}.vcxproj.user" 
"<?xml version=\"1.0\" encoding=\"utf-8\"?> \
<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"> \
  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\"> \
    <LocalDebuggerWorkingDirectory>${directory}</LocalDebuggerWorkingDirectory> \
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor> \
  </PropertyGroup> \
  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\"> \
    <LocalDebuggerWorkingDirectory>${directory}</LocalDebuggerWorkingDirectory> \
    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor> \
  </PropertyGroup> \
</Project>"
        )
    else()
        message(FATAL_ERROR "This compiler is not supported!")
    endif()
ENDFUNCTION() # SET_DEBUG_DIRECTORY
