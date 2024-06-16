cmake_minimum_required(VERSION 3.1...3.25)

project(
    Silverback
    VERSION 0.1.0
    LANGUAGES C CXX
)


set(CMAKE_CXX_STANDARD 20)

# options to build tests and demo.
option(SILVERBACK_BUILD_UNIT_TESTS "Build unit tests" ON)
option(SILVERBACK_BUILD_DEMO "Build demo application" OFF)


# Builds unit tests if the option is set to ON.
if(SILVERBACK_BUILD_UNIT_TESTS)
    add_subdirectory(External/GoogleTest)
    add_subdirectory(Units)
endif()

# Builds Demo if the option is set to ON.
if(SILVERBACK_BUILD_DEMO)
    add_subdirectory(External/Glad)
    add_subdirectory(External/GLFW)
    add_subdirectory(Demo)
endif()

