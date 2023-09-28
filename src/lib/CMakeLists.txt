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

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${PROJECT_SOURCE_DIR}/contrib/fmt/include"
        "${PROJECT_SOURCE_DIR}/contrib/inja/src"
)

target_link_libraries(${PROJECT_NAME}
    Boost::filesystem Boost::regex Boost::system Boost::spirit
    nlohmann_json::nlohmann_json
)