project(tgafunc_test C)

add_executable(${PROJECT_NAME} test.c)

# Copy the test images to binary folder.
file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} tgafunc)
