cmake_minimum_required(VERSION 3.1...3.25)

project(
    Silverback
    VERSION 0.1.0
    LANGUAGES C CXX
)

#enable_testing()

set(CMAKE_CXX_STANDARD 20)

option(SILVERBACK_BUILD_UNIT_TESTS "Build unit tests" ON)
option(SILVERBACK_BUILD_DEMO "Build demo application" OFF)


if(SILVERBACK_BUILD_UNIT_TESTS)
    add_subdirectory(External/GoogleTest)
    add_subdirectory(Units)
endif()

if(SILVERBACK_BUILD_DEMO)
    add_subdirectory(External/Glad)
    add_subdirectory(External/GLFW)
    add_subdirectory(Demo)
endif()


#include(CTest)