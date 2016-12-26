cmake_minimum_required(VERSION 3.0.0)

file(GLOB lib
    "*.cpp"
)

add_library (${PROJECT_NAME} ${lib})