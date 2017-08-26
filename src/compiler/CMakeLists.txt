cmake_minimum_required(VERSION 3.8.0)

add_executable (${PROJECT_NAME}_compiler
    main.cpp
)

target_link_libraries(${PROJECT_NAME}_compiler ${Boost_LIBRARIES} ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}_compiler PRIVATE ${PROJECT_SOURCE_DIR}/contrib/cxxopts/include)

target_compile_definitions(${PROJECT_NAME}_compiler
    PRIVATE
        CXXOPTS_NO_RTTI=1
)

# TODO: Remove once Visual Studio uses CMake 3.8 or higher
 if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")		
    set(MY_CXX_FLAGS_LIST		
        /std:c++latest
    )		
    string(REPLACE ";" " " MY_CXX_FLAGS "${MY_CXX_FLAGS_LIST}")
 		
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}")
 endif()
