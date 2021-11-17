#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int absolute_check()
{
  const char *relative_paths[] = {"..", "test", "test/test", "../another_test",
    "./simple", ".././simple"};
  const char *absolute_paths[] = {"/", "/test", "/../test/", "/../another_test",
    "/./simple", "/.././simple"};
  size_t i;

  cwk_path_set_style(CWK_STYLE_UNIX);

  for (i = 0; i < ARRAY_SIZE(relative_paths); ++i) {
    if (cwk_path_is_absolute(relative_paths[i])) {
      return EXIT_FAILURE;
    }
  }

  for (i = 0; i < ARRAY_SIZE(absolute_paths); ++i) {
    if (!cwk_path_is_absolute(absolute_paths[i])) {
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int absolute_too_far()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute("/hello/there", "../../../../../", buffer,
    sizeof(buffer));

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_normalization()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute("/hello//../there", "test//thing", buffer,
    sizeof(buffer));

  if (length != 17) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/there/test/thing") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_mixed()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute("hello/there", "/test", buffer,
    sizeof(buffer));

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_unix_relative_base()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute("hello/there", "test", buffer, sizeof(buffer));

  if (length != 17) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/hello/there/test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_windows_relative_base()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  length = cwk_path_get_absolute("hello\\there", "test", buffer, sizeof(buffer));

  if (length != 17) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "\\hello\\there\\test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_absolute_path()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute("/hello/there", "/test", buffer,
    sizeof(buffer));

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/test") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int absolute_simple()
{
  char buffer[FILENAME_MAX];
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  length = cwk_path_get_absolute("/hello/there", "..", buffer, sizeof(buffer));

  if (length != 6) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/hello") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int absolute_buffer_reuse()
{
  char path[FILENAME_MAX];

  memset(path, 1, FILENAME_MAX);
  path[0] = '\0';

  cwk_path_set_style(CWK_STYLE_UNIX);

  cwk_path_get_absolute(path, "/", path, FILENAME_MAX);
  if (strcmp(path, "/") != 0) {
    return EXIT_FAILURE;
  }
  cwk_path_get_absolute(path, "see", path, FILENAME_MAX);
  if (strcmp(path, "/see") != 0) {
    return EXIT_FAILURE;
  }

  cwk_path_get_absolute(path, "dog", path, FILENAME_MAX);
  if (strcmp(path, "/see/dog") != 0) {
    return EXIT_FAILURE;
  }

  cwk_path_get_absolute(path, "..", path, FILENAME_MAX);
  if (strcmp(path, "/see") != 0) {
    return EXIT_FAILURE;
  }

  cwk_path_get_absolute(path, "cat", path, FILENAME_MAX);
  if (strcmp(path, "/see/cat") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
