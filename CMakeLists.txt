cmake_minimum_required(VERSION 3.25.0)
project(flatmessage)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories (${PROJECT_SOURCE_DIR}/src/include)

include(FetchContent)

#find_package(Boost 1.63 COMPONENTS filesystem regex system REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_IOSTREAMS_ENABLE_ZLIB OFF)

message("Downloading libraries. This may take a while, please be patient...")

FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.83.0
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(Boost nlohmann_json)

message("... downloading done!")

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
