cmake_minimum_required(VERSION 3.8.0)

add_executable (test_${PROJECT_NAME}
    main.cpp
    compiler.cpp
    parse_expression.cpp
    template_generator.cpp
)

target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME} Boost::filesystem Boost::regex Boost::system)

target_include_directories(test_${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/contrib/testinator/src/include"
        "${PROJECT_SOURCE_DIR}/contrib/fmt/include")

target_compile_definitions(test_${PROJECT_NAME} 
    PRIVATE FMT_HEADER_ONLY=1
    _HAS_AUTO_PTR_ETC=1 # TODO: Remove once boost no longer uses std::unary_function
)

set_target_properties(test_${PROJECT_NAME}
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

include(Testinator)
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parse_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/generate_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/compiler_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})