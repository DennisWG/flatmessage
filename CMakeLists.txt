cmake_minimum_required(VERSION 3.7.0)
project(playground)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories (${PROJECT_SOURCE_DIR}/src/include)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.63 COMPONENTS filesystem regex system REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost
    PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

# Testing
include(CTest)
enable_testing()

add_subdirectory (src/lib)
add_subdirectory (src/test)
