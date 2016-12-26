cmake_minimum_required(VERSION 3.0.0)

add_executable (test_${PROJECT_NAME} main.cpp)
target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})