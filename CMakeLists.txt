cmake_minimum_required(VERSION 3.5)

project(foolrenderer C)

add_subdirectory(external/tgafunc)

add_executable(${PROJECT_NAME}
    foolrenderer/main.c
    foolrenderer/math/vector.c
    foolrenderer/math/matrix.c
)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/foolrenderer
)

target_link_libraries(${PROJECT_NAME} tgafunc)
