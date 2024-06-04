add_executable(DeltaUnitTests main.cpp)

target_include_directories(DeltaUnitTests 
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GoogleTest/googletest/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

target_link_libraries(DeltaUnitTests gtest_main ${PROJECT_NAME})