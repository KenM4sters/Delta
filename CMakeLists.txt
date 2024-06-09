cmake_minimum_required(VERSION 3.1...3.25)

project(
    Silverback
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(External/GoogleTest)
add_subdirectory(Sources)
add_subdirectory(Tests)