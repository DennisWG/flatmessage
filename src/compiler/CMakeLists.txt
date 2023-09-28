cmake_minimum_required(VERSION 3.8.0)

add_executable (${PROJECT_NAME}_compiler
    main.cpp
)

target_link_libraries(${PROJECT_NAME}_compiler ${PROJECT_NAME} Boost::filesystem Boost::regex Boost::system)

target_include_directories(${PROJECT_NAME}_compiler PRIVATE ${PROJECT_SOURCE_DIR}/contrib/cxxopts/include)

target_compile_definitions(${PROJECT_NAME}_compiler
    PRIVATE
        CXXOPTS_NO_RTTI=1
)

target_compile_features(${PROJECT_NAME}_compiler PRIVATE cxx_std_23)


install(TARGETS ${PROJECT_NAME}_compiler DESTINATION ${CMAKE_INSTALL_PREFIX})