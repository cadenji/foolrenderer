cmake_minimum_required(VERSION 3.9.2)

# set project name
project(cwalk
  VERSION 1.2.5
  DESCRIPTION "A simple path library"
  LANGUAGES C)

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EnableWarnings)
include(CTest)
include(CreateTestList)
include(CMakePackageConfigHelpers)

# configure requirements
set(CMAKE_C_STANDARD 11)

# setup target and directory names
set(INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

# enable coverage if requested
if(ENABLE_COVERAGE)
  message("-- Coverage enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# enable sanitizer
if(ENABLE_SANITIZER)
  message("-- Sanitizer enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=${ENABLE_SANITIZER}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=${ENABLE_SANITIZER}")
endif()

# add the main executable
add_library(cwalk
  "${INCLUDE_DIRECTORY}/cwalk.h"
  "${SOURCE_DIRECTORY}/cwalk.c")
enable_warnings(cwalk)
target_include_directories(cwalk PUBLIC
  $<BUILD_INTERFACE:${INCLUDE_DIRECTORY}>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(cwalk PROPERTIES PUBLIC_HEADER "${INCLUDE_DIRECTORY}/cwalk.h")
set_target_properties(cwalk PROPERTIES DEFINE_SYMBOL CWK_EXPORTS)

# add shared library macro
if(BUILD_SHARED_LIBS)
  target_compile_definitions(cwalk PUBLIC CWK_SHARED)
endif()

# enable tests
if(ENABLE_TESTS)
  message("-- Tests enabled")
  enable_testing()

  create_test_list(DEFAULT cwalktest)
  create_test(DEFAULT absolute simple)
  create_test(DEFAULT absolute absolute_path)
  create_test(DEFAULT absolute unix_relative_base)
  create_test(DEFAULT absolute windows_relative_base)
  create_test(DEFAULT absolute mixed)
  create_test(DEFAULT absolute normalization)
  create_test(DEFAULT absolute too_far)
  create_test(DEFAULT absolute check)
  create_test(DEFAULT absolute buffer_reuse)
  create_test(DEFAULT basename simple)
  create_test(DEFAULT basename empty)
  create_test(DEFAULT basename trailing_separator)
  create_test(DEFAULT basename trailing_separators)
  create_test(DEFAULT basename no_separators)
  create_test(DEFAULT basename special_directories)
  create_test(DEFAULT basename root)
  create_test(DEFAULT basename windows)
  create_test(DEFAULT basename change_simple)
  create_test(DEFAULT basename change_empty_path)
  create_test(DEFAULT basename change_only_root)
  create_test(DEFAULT basename change_empty_basename)
  create_test(DEFAULT basename change_relative)
  create_test(DEFAULT basename change_trim)
  create_test(DEFAULT basename change_trim_only_root)
  create_test(DEFAULT dirname simple)
  create_test(DEFAULT dirname empty)
  create_test(DEFAULT dirname trailing_separator)
  create_test(DEFAULT dirname trailing_separators)
  create_test(DEFAULT dirname no_separators)
  create_test(DEFAULT dirname special_directories)
  create_test(DEFAULT dirname root)
  create_test(DEFAULT dirname three_segments)
  create_test(DEFAULT dirname relative)
  create_test(DEFAULT extension get_simple)
  create_test(DEFAULT extension get_without)
  create_test(DEFAULT extension get_first)
  create_test(DEFAULT extension get_last)
  create_test(DEFAULT extension get_multiple)
  create_test(DEFAULT extension check_simple)
  create_test(DEFAULT extension check_empty)
  create_test(DEFAULT extension check_without)
  create_test(DEFAULT extension change_simple)
  create_test(DEFAULT extension change_no_basename)
  create_test(DEFAULT extension change_no_extension)
  create_test(DEFAULT extension change_with_dot)
  create_test(DEFAULT extension change_overlap)
  create_test(DEFAULT extension change_overlap_long)
  create_test(DEFAULT extension change_hidden_file)
  create_test(DEFAULT extension change_with_trailing_slash)
  create_test(DEFAULT guess empty_string)
  create_test(DEFAULT guess windows_root)
  create_test(DEFAULT guess unix_root)
  create_test(DEFAULT guess windows_separator)
  create_test(DEFAULT guess unix_separator)
  create_test(DEFAULT guess hidden_file)
  create_test(DEFAULT guess extension)
  create_test(DEFAULT guess unguessable)
  create_test(DEFAULT intersection simple)
  create_test(DEFAULT intersection trailing_separator)
  create_test(DEFAULT intersection double_separator)
  create_test(DEFAULT intersection empty)
  create_test(DEFAULT intersection unequal_roots)
  create_test(DEFAULT intersection relative_absolute_mix)
  create_test(DEFAULT intersection same_roots)
  create_test(DEFAULT intersection one_root_only)
  create_test(DEFAULT intersection relative_base)
  create_test(DEFAULT intersection relative_other)
  create_test(DEFAULT intersection skipped_end)
  create_test(DEFAULT is_absolute absolute)
  create_test(DEFAULT is_absolute unc)
  create_test(DEFAULT is_absolute device_unc)
  create_test(DEFAULT is_absolute device_dot)
  create_test(DEFAULT is_absolute device_question_mark)
  create_test(DEFAULT is_absolute relative)
  create_test(DEFAULT is_absolute windows_backslash)
  create_test(DEFAULT is_absolute windows_slash)
  create_test(DEFAULT is_absolute unix_backslash)
  create_test(DEFAULT is_absolute unix_drive)
  create_test(DEFAULT is_absolute absolute_drive)
  create_test(DEFAULT is_absolute relative_drive)
  create_test(DEFAULT is_absolute relative_windows)
  create_test(DEFAULT is_relative absolute)
  create_test(DEFAULT is_relative unc)
  create_test(DEFAULT is_relative device_unc)
  create_test(DEFAULT is_relative device_dot)
  create_test(DEFAULT is_relative device_question_mark)
  create_test(DEFAULT is_relative relative)
  create_test(DEFAULT is_relative windows_backslash)
  create_test(DEFAULT is_relative windows_slash)
  create_test(DEFAULT is_relative unix_backslash)
  create_test(DEFAULT is_relative unix_drive)
  create_test(DEFAULT is_relative absolute_drive)
  create_test(DEFAULT is_relative relative_drive)
  create_test(DEFAULT is_relative relative_windows)
  create_test(DEFAULT join simple)
  create_test(DEFAULT join navigate_back)
  create_test(DEFAULT join empty)
  create_test(DEFAULT join two_absolute)
  create_test(DEFAULT join two_unc)
  create_test(DEFAULT join with_two_roots)
  create_test(DEFAULT join back_after_root)
  create_test(DEFAULT join relative_back_after_root)
  create_test(DEFAULT join multiple)
  create_test(DEFAULT normalize do_nothing)
  create_test(DEFAULT normalize navigate_back)
  create_test(DEFAULT normalize relative_too_far)
  create_test(DEFAULT normalize absolute_too_far)
  create_test(DEFAULT normalize terminated)
  create_test(DEFAULT normalize double_separator)
  create_test(DEFAULT normalize remove_current)
  create_test(DEFAULT normalize mixed)
  create_test(DEFAULT normalize overlap)
  create_test(DEFAULT normalize empty)
  create_test(DEFAULT normalize only_separators)
  create_test(DEFAULT normalize back_after_root)
  create_test(DEFAULT relative simple)
  create_test(DEFAULT relative relative)
  create_test(DEFAULT relative long_base)
  create_test(DEFAULT relative long_target)
  create_test(DEFAULT relative equal)
  create_test(DEFAULT relative same_base)
  create_test(DEFAULT relative base_skipped_end)
  create_test(DEFAULT relative target_skipped_end)
  create_test(DEFAULT relative base_div_skipped_end)
  create_test(DEFAULT relative target_div_skipped_end)
  create_test(DEFAULT relative skip_all)
  create_test(DEFAULT relative different_roots)
  create_test(DEFAULT relative relative_and_absolute)
  create_test(DEFAULT relative check)
  create_test(DEFAULT relative root_path_unix)
  create_test(DEFAULT relative root_path_windows)
  create_test(DEFAULT root absolute)
  create_test(DEFAULT root unc)
  create_test(DEFAULT root device_unc)
  create_test(DEFAULT root device_dot)
  create_test(DEFAULT root device_question_mark)
  create_test(DEFAULT root relative)
  create_test(DEFAULT root windows_backslash)
  create_test(DEFAULT root windows_slash)
  create_test(DEFAULT root unix_backslash)
  create_test(DEFAULT root unix_drive)
  create_test(DEFAULT root absolute_drive)
  create_test(DEFAULT root relative_drive)
  create_test(DEFAULT root relative_windows)
  create_test(DEFAULT root change_simple)
  create_test(DEFAULT root change_empty)
  create_test(DEFAULT root change_separators)
  create_test(DEFAULT root change_overlapping)
  create_test(DEFAULT root change_without_root)
  create_test(DEFAULT segment first)
  create_test(DEFAULT segment last)
  create_test(DEFAULT segment next)
  create_test(DEFAULT segment next_too_far)
  create_test(DEFAULT segment previous_absolute)
  create_test(DEFAULT segment previous_relative)
  create_test(DEFAULT segment previous_absolute_one_char_first)
  create_test(DEFAULT segment previous_relative_one_char_first)
  create_test(DEFAULT segment previous_too_far)
  create_test(DEFAULT segment previous_too_far_root)
  create_test(DEFAULT segment type)
  create_test(DEFAULT segment back_with_root)
  create_test(DEFAULT segment change_simple)
  create_test(DEFAULT segment change_first)
  create_test(DEFAULT segment change_last)
  create_test(DEFAULT segment change_trim)
  create_test(DEFAULT segment change_empty)
  create_test(DEFAULT segment change_with_separator)
  create_test(DEFAULT segment change_overlap)
  create_test(DEFAULT windows change_style)
  create_test(DEFAULT windows get_root)
  create_test(DEFAULT windows get_unc_root)
  create_test(DEFAULT windows get_root_separator)
  create_test(DEFAULT windows get_root_relative)
  create_test(DEFAULT windows intersection_case)
  create_test(DEFAULT windows root_backslash)
  create_test(DEFAULT windows root_empty)
  write_test_file(DEFAULT "${TEST_DIRECTORY}/tests.h")

  add_executable(cwalktest
    "${TEST_DIRECTORY}/main.c"
    "${TEST_DIRECTORY}/absolute_test.c"
    "${TEST_DIRECTORY}/basename_test.c"
    "${TEST_DIRECTORY}/dirname_test.c"
    "${TEST_DIRECTORY}/extension_test.c"
    "${TEST_DIRECTORY}/guess_test.c"
    "${TEST_DIRECTORY}/intersection_test.c"
    "${TEST_DIRECTORY}/is_absolute_test.c"
    "${TEST_DIRECTORY}/is_relative_test.c"
    "${TEST_DIRECTORY}/join_test.c"
    "${TEST_DIRECTORY}/normalize_test.c"
    "${TEST_DIRECTORY}/relative_test.c"
    "${TEST_DIRECTORY}/root_test.c"
    "${TEST_DIRECTORY}/segment_test.c"
    "${TEST_DIRECTORY}/windows_test.c")
  enable_warnings(cwalktest)
    
  target_link_libraries(cwalktest PRIVATE cwalk)
endif()

write_basic_package_version_file("CwalkConfigVersion.cmake"
  VERSION ${cwalk_VERSION}
  COMPATIBILITY SameMajorVersion)

install(TARGETS cwalk
  EXPORT CwalkTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)

install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CwalkConfig.cmake"
  DESTINATION lib/cmake/cwalk)

install(EXPORT CwalkTargets
  FILE CwalkTargets.cmake
  DESTINATION lib/cmake/cwalk)
