add_executable(SilverbackUnitTests main.cpp)

target_include_directories(SilverbackUnitTests 
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GoogleTest/googletest/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

target_link_libraries(SilverbackUnitTests gtest_main ${PROJECT_NAME})