cmake_minimum_required(VERSION 3.7.0)

add_library (${PROJECT_NAME} 
    compiler.cpp
    parser.cpp
    parser/expression.cpp
    ast/printer.cpp
    generator/template_generator.cpp
)

target_compile_definitions(${PROJECT_NAME} 
    PRIVATE FMT_HEADER_ONLY=1
)

# TODO: Remove once Visual Studio uses CMake 3.8 or higher
 if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")		
    set(MY_CXX_FLAGS_LIST		
        /std:c++latest		
    )		
    string(REPLACE ";" " " MY_CXX_FLAGS "${MY_CXX_FLAGS_LIST}")		
 		
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")		
 endif()

target_compile_features(${PROJECT_NAME}
    PUBLIC cxx_std_17
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/contrib/fmt
        ${PROJECT_SOURCE_DIR}/contrib/inja/src
        ${nlohmann_json_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    boost
)

#set_target_properties(${PROJECT_NAME}
#    PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
#)