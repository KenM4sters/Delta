add_executable(SilverbackDemo main.cpp)

# Tells the compiler where to look for files so that we don't have to backout and 
# forward into so many different locations when including headers.
# Not required, but useful.
target_include_directories(SilverbackDemo 
    PUBLIC ${CMAKE_SOURCE_DIR}/Sources/
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GLFW/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/External/Glad/include/
    PUBLIC ${CMAKE_SOURCE_DIR}/External/GLM/
)

# Links the SilverbackDemo executable against the glfw and glad libraries.
# Required only for this demo, not for Silverback itself (which is header only - no library).
target_link_libraries(SilverbackDemo glfw glad glm)


# Address Sanitizer config for detailed error info.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
