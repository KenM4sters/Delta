add_executable(UnitTest main.cpp)

target_include_directories(UnitTest 
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GoogleTest/googletest/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

target_link_libraries(UnitTest gtest_main ECSSources)