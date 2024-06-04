cmake_minimum_required(VERSION 3.1...3.25)

project(
    ECSSources
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CXX_STANDARD 17)

add_library(ECSSources
    Component.cpp
    System.cpp
    ECS.cpp
)

target_include_directories(ECSSources 
    PUBLIC ${CMAKE_SOURCE_DIR}/ECSSources/
    PUBLIC ${CMAKE_SOURCE_DIR}/External/Essence/Sources/
)

target_link_libraries(ECSSources Sources)

