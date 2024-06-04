add_executable(DeltaCustomTests main.cpp)

target_include_directories(DeltaCustomTests 
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
)

target_link_libraries(DeltaCustomTests ${PROJECT_NAME})