cmake_minimum_required(VERSION 3.7.0)
project(flatmessage)

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

if(MSVC)
  # Use at least verion 3.8.1 of FindBoost to fix some issues. Especialy in MSVC with fixed Toolset issues.
  # This is a workaround while VisualStudio does not embed higher cmake version than 3.7.2
  if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} LESS 3.8.1)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/FindBoost)
    message(STATUS "FindBoost: most recent version of FindBoost is used.")
  endif()
endif()

add_subdirectory (src/lib)
add_subdirectory (src/test)
add_subdirectory (src/compiler)
