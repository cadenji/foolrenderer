cmake_minimum_required(VERSION 3.8)

project(foolrenderer C)

add_executable(${PROJECT_NAME}
    foolrenderer/graphics/color.c
    foolrenderer/graphics/framebuffer.c
    foolrenderer/graphics/rasterizer.c
    foolrenderer/graphics/shader_context.c
    foolrenderer/graphics/texture.c
    foolrenderer/math/vector.c
    foolrenderer/math/matrix.c
    foolrenderer/math/math_utility.c
    foolrenderer/shaders/basic.c
    foolrenderer/shaders/standard.c
    foolrenderer/shaders/shadow_casting.c
    foolrenderer/utilities/image.c
    foolrenderer/utilities/mesh.c
    foolrenderer/main.c
)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)

# Set strict warning level for different compilers.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/foolrenderer
)

# Copy the assets to binary folder.
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Check if some libraries exist and link them if they exist.
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(m pow "" HAVE_LIB_M)
if(HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif()

# Add external libraries
add_subdirectory(external/tgafunc)
add_subdirectory(external/fast_obj)

target_link_libraries(${PROJECT_NAME}
    ${EXTRA_LIBS}
    tgafunc
    fast_obj_lib
)
