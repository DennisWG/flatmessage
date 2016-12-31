cmake_minimum_required(VERSION 3.0.0)

file(GLOB tests
    "*.cpp"
)

add_executable (test_${PROJECT_NAME} ${tests})
target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
target_link_libraries(test_${PROJECT_NAME} ${Boost_LIBRARIES})
message("libs: ${Boost_LIBRARIES}")
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})