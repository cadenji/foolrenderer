#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int basename_change_trim_only_root()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_basename("/", "///another.txt///", buffer,
    sizeof(buffer));
  if (n != 12) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/another.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_change_trim()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_basename("/test.txt", "///another.txt///", buffer,
    sizeof(buffer));
  if (n != 12) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "/another.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_change_relative()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_basename("../test.txt", "another.txt", buffer,
    sizeof(buffer));
  if (n != 14) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "../another.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_change_empty_basename()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_basename("C:\\test.txt", "", buffer, sizeof(buffer));
  if (n != 3) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "C:\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_change_only_root()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_basename("C:\\", "another.txt", buffer, sizeof(buffer));
  if (n != 14) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "C:\\another.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_change_empty_path()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_basename("", "another.txt", buffer, sizeof(buffer));
  if (n != 11) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "another.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_change_simple()
{
  size_t n;
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_basename("C:\\test.txt", "another.txt", buffer,
    sizeof(buffer));
  if (n != 14) {
    return EXIT_FAILURE;
  }

  if (strcmp(buffer, "C:\\another.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_windows()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_WINDOWS);
  path = "C:\\path\\test.txt";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strcmp(basename, "test.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_root()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  if (basename != NULL) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_special_directories()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "..";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 2) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "..", length) != 0) {
    return EXIT_FAILURE;
  }

  path = ".";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, ".", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_no_separators()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "file_name";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 9) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "file_name", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_trailing_separators()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt////";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "path.txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_trailing_separator()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt/";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "path.txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_empty()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  if (basename != NULL) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int basename_simple()
{
  const char *path, *basename;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt";
  cwk_path_get_basename(path, &basename, &length);

  if (length != 8) {
    return EXIT_FAILURE;
  }

  if (strncmp(basename, "path.txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
