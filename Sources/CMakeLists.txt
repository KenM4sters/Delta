cmake_minimum_required(VERSION 3.1...3.25)

project(
    ${PROJECT_NAME}
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CXX_STANDARD 17)

add_library(${PROJECT_NAME}
    Component.cpp
    System.cpp
    ECS.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)


