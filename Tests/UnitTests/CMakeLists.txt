
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

add_executable(SilverbackUnitTests 
    ${SOURCES}
)

target_include_directories(SilverbackUnitTests 
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GoogleTest/googletest/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
) 

target_link_libraries(SilverbackUnitTests gtest_main)