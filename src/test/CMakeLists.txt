cmake_minimum_required(VERSION 3.7.0)

add_executable (test_${PROJECT_NAME}
    main.cpp
    compiler.cpp
    parse_expression.cpp
    template_generator.cpp
)

target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
target_link_libraries(test_${PROJECT_NAME} ${Boost_LIBRARIES})

target_include_directories(test_${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/contrib/testinator/src/include)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    include(VsDebugDir)
    SET_DEBUG_DIRECTORY(test_${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# TODO: uncomment below and remove above once Visual Studio updates to CMake 3.8 or higher
#set_target_properties(test_${PROJECT_NAME}
#    PROPERTIES
#        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#)

include(Testinator)
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/parse_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/generate_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/compiler_expression DESTINATION ${CMAKE_CURRENT_BINARY_DIR})