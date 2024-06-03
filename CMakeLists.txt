cmake_minimum_required(VERSION 3.1...3.25)

project(
    Delta
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CXX_STANDARD 17)

add_subdirectory(External/GoogleTest)
add_subdirectory(Sources)
add_subdirectory(Tests)