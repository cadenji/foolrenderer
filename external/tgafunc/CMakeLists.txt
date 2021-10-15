cmake_minimum_required(VERSION 3.5)

project(tgafunc C)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(TGAFUNC_STANDALONE TRUE)
endif()

option(TGAFUNC_BUILD_TESTS "Build the tgafunc test programs" ${TGAFUNC_STANDALONE})

add_library(${PROJECT_NAME} STATIC tgafunc.c)
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)

# Set strict warning level for different compilers.
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}
)

if(TGAFUNC_BUILD_TESTS)
    add_subdirectory(test)
endif()
