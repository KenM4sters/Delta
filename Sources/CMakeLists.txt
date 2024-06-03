cmake_minimum_required(VERSION 3.1...3.25)

project(
    Sources
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CXX_STANDARD 17)

add_library(Sources
    Entity.cpp
)

target_include_directories(Sources 
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

