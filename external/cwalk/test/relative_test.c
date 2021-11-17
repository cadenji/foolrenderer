#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int relative_root_path_windows()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  length = cwk_path_get_relative("C:\\this\\is\\path_one", "C:\\", result,
    sizeof(result));
  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "..\\..\\..") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_root_path_unix()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/this/is/path_one", "/", result,
    sizeof(result));
  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "../../..") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_check()
{
  const char *relative_paths[] = {"..", "test", "test/test", "../another_test",
    "./simple", ".././simple"};
  const char *absolute_paths[] = {"/", "/test", "/../test/", "/../another_test",
    "/./simple", "/.././simple"};
  size_t i;

  cwk_path_set_style(CWK_STYLE_UNIX);

  for (i = 0; i < ARRAY_SIZE(relative_paths); ++i) {
    if (!cwk_path_is_relative(relative_paths[i])) {
      return EXIT_FAILURE;
    }
  }

  for (i = 0; i < ARRAY_SIZE(absolute_paths); ++i) {
    if (cwk_path_is_relative(absolute_paths[i])) {
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int relative_relative_and_absolute()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  *result = 1;

  length = cwk_path_get_relative("./foo", "/bar", result, sizeof(result));

  if (length != 0) {
    return EXIT_FAILURE;
  }

  if (*result != '\0') {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_different_roots()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  *result = 1;

  length = cwk_path_get_relative("C:/path/same", "D:/path/same", result,
    sizeof(result));

  if (length != 0) {
    return EXIT_FAILURE;
  }

  if (*result != '\0') {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_skip_all()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/../../", "/../../", result, sizeof(result));
  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, ".") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_target_div_skipped_end()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/same", "/path/not_same/ho/..", result,
    sizeof(result));
  if (length != 11) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "../not_same") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_base_div_skipped_end()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/not_same/ho/..", "/path/same", result,
    sizeof(result));
  if (length != 7) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "../same") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_target_skipped_end()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/same", "/path/same/ho/..", result,
    sizeof(result));
  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, ".") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_base_skipped_end()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/same/ho/..", "/path/same", result,
    sizeof(result));
  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, ".") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_equal()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/same", "/path/same", result,
    sizeof(result));
  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, ".") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_same_base()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/dir/dir", "/dir/dir3/file", result,
    sizeof(result));
  if (length != 12) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "../dir3/file") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_long_target()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/long/one", "/path/long/one/two", result,
    sizeof(result));
  if (length != 3) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "two") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_long_base()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/path/long/one/two", "/path/long/one", result,
    sizeof(result));
  if (length != 2) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "..") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_relative()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("./this/is/path_one", "./this/is/path_two",
    result, sizeof(result));
  if (length != 11) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "../path_two") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int relative_simple()
{
  char result[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);

  length = cwk_path_get_relative("/this/is/path_one", "/this/is/path_two",
    result, sizeof(result));
  if (length != 11) {
    return EXIT_FAILURE;
  }

  if (strcmp(result, "../path_two") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
