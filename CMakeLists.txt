cmake_minimum_required(VERSION 3.0.0)
project(playground)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories ("${PROJECT_SOURCE_DIR}/src/include")

# external submodules
include_directories ("${PROJECT_SOURCE_DIR}/contrib/testinator/src/include")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.63.0 COMPONENTS filesystem regex system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# Testing
include(CTest)
enable_testing()

# Pipe separate tests into ctest
# Adapted from https://github.com/elbeno/skeleton/blob/master/CMakeLists.txt
macro(ADD_INDIVIDUAL_TESTS executable type suffix)
  set(test_path $ENV{PATH})
  get_target_property(target_files ${executable} SOURCES)
  foreach(source ${target_files})
    string(REGEX MATCH .*cpp source "${source}")
    if(source)
      file(READ "${source}" contents)
      string(REGEX MATCHALL "DEF_${type}[ ]*[(][ ]*[^, ]+[ ]*,[ ]*[^,) ]+[ ]*[),]" found_tests ${contents})
      foreach(hit ${found_tests})
        string(REGEX REPLACE "DEF_${type}[ ]*[(][ ]*([^, ]+)[ ]*,[ ]*[^,) ]+[ ]*[),]" "\\1" tname ${hit})
        string(REGEX REPLACE "DEF_${type}[ ]*[(][ ]*[^, ]+[ ]*,[ ]*([^,) ]+)[ ]*[),]" "\\1" sname ${hit})
        set(test_name ${executable}.${sname}.${tname}${suffix})
        add_test(NAME ${test_name}
          COMMAND "${executable}" --testName=${tname}${suffix} --suiteName=${sname})
        set_tests_properties(${test_name} PROPERTIES TIMEOUT 30 ENVIRONMENT "PATH=${test_path}")
      endforeach()
    endif()
  endforeach()
endmacro()

macro(ADD_TESTINATOR_TESTS executable)
  ADD_INDIVIDUAL_TESTS(${executable} "TEST" "")
  ADD_INDIVIDUAL_TESTS(${executable} "TIMED_TEST" "")
  ADD_INDIVIDUAL_TESTS(${executable} "PROPERTY" "Property")
  ADD_INDIVIDUAL_TESTS(${executable} "COMPLEXITY_PROPERTY" "ComplexityProperty")
endmacro()

# Compile flags
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(MY_CXX_FLAGS_LIST
	/std:c++latest
    )
  string(REPLACE ";" " " MY_CXX_FLAGS "${MY_CXX_FLAGS_LIST}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
endif()

add_subdirectory (src/lib)
add_subdirectory (src/test)
