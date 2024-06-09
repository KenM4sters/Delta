add_executable(SilverbackCustomTests main.cpp)

target_include_directories(SilverbackCustomTests 
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

target_link_libraries(SilverbackCustomTests ${PROJECT_NAME})