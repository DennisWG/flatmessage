cmake_minimum_required(VERSION 3.8.0)

add_executable (test_${PROJECT_NAME}
    main.cpp
    compiler.cpp
    parse_expression.cpp
    template_generator.cpp
)

target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
target_link_libraries(test_${PROJECT_NAME} ${Boost_LIBRARIES})

target_include_directories(test_${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/contrib/testinator/src/include)

target_compile_definitions(test_${PROJECT_NAME} 
    PRIVATE FMT_HEADER_ONLY=1
    _HAS_AUTO_PTR_ETC=1 # TODO: Remove once boost no longer uses std::unary_function
)

set_target_properties(test_${PROJECT_NAME}
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# TODO: Remove once Visual Studio uses CMake 3.8 or higher
 if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")		
    set(MY_CXX_FLAGS_LIST		
        /std:c++latest
    )		
    string(REPLACE ";" " " MY_CXX_FLAGS "${MY_CXX_FLAGS_LIST}")
 		
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
 endif()

include(Testinator)
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parse_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/generate_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/compiler_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})