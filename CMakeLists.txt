cmake_minimum_required(VERSION 3.5)

project(foolrenderer C)

add_executable(${PROJECT_NAME}
    foolrenderer/main.c
    foolrenderer/mesh.c
    foolrenderer/framebuffer.c
    foolrenderer/graphics.c
    foolrenderer/texture.c
    foolrenderer/shader_context.c
    foolrenderer/math/vector.c
    foolrenderer/math/matrix.c
    foolrenderer/math/math_utility.c
    foolrenderer/shaders/basic.c
)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)

# Set strict warning level for different compilers.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/foolrenderer
)

# Copy the assets to binary folder.
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Add external libraries
add_subdirectory(external/tgafunc)

add_library(tinyobjloader-c INTERFACE)
target_include_directories(tinyobjloader-c INTERFACE external/tinyobjloader-c)

add_subdirectory(external/cwalk)

target_link_libraries(${PROJECT_NAME}
    tgafunc
    tinyobjloader-c
    cwalk
)
