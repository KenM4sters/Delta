file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

add_executable(SilverbackUnitTests 
    ${SOURCES}
)

target_include_directories(SilverbackUnitTests 
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GoogleTest/googletest/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
) 

target_link_libraries(SilverbackUnitTests gtest_main)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
add_test(NAME SilverbackUnitTests COMMAND SilverbackUnitTests --gtest_color=yes --gtest_output=xml:report.xml --gtest_print_time)
