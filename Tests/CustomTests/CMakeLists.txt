add_executable(CustomTest main.cpp)

target_include_directories(CustomTest 
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

target_link_libraries(CustomTest ECSSources)