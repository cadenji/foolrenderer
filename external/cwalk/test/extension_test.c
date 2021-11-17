#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int extension_change_with_trailing_slash()
{
  char buffer[FILENAME_MAX] = "/folder/file.txt/";
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension(buffer, ".md", buffer, sizeof(buffer));
  if (n != 16) {
    return EXIT_FAILURE;
  }

  if (strcmp("/folder/file.md/", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_hidden_file()
{
  char buffer[FILENAME_MAX] = "/folder/.hidden_file.txt";
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension(buffer, ".md", buffer, sizeof(buffer));
  if (n != 23) {
    return EXIT_FAILURE;
  }

  if (strcmp("/folder/.hidden_file.md", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_overlap_long()
{
  char buffer[FILENAME_MAX] = "/test.txt/";
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension(buffer, ".text", buffer, sizeof(buffer));
  if (n != 11) {
    return EXIT_FAILURE;
  }

  if (strcmp("/test.text/", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_overlap()
{
  char buffer[FILENAME_MAX] = "/test.txt/";
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension(buffer, ".md", buffer, sizeof(buffer));
  if (n != 9) {
    return EXIT_FAILURE;
  }

  if (strcmp("/test.md/", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_with_dot()
{
  char buffer[FILENAME_MAX];
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension("/test.txt", ".md", buffer, sizeof(buffer));
  if (n != 8) {
    return EXIT_FAILURE;
  }

  if (strcmp("/test.md", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_remove()
{
  char buffer[FILENAME_MAX];
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension("/test.txt", "", buffer, sizeof(buffer));
  if (n != 5) {
    return EXIT_FAILURE;
  }

  if (strcmp("/test", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_no_extension()
{
  char buffer[FILENAME_MAX] = "C:\\file";
  size_t n;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_extension(buffer, ".md", buffer, sizeof(buffer));
  if (n != 10) {
    return EXIT_FAILURE;
  }

  if (strcmp("C:\\file.md", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_no_basename()
{
  char buffer[FILENAME_MAX] = "C:\\";
  size_t n;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  n = cwk_path_change_extension(buffer, ".md", buffer, sizeof(buffer));
  if (n != 6) {
    return EXIT_FAILURE;
  }

  if (strcmp("C:\\.md", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_change_simple()
{
  char buffer[FILENAME_MAX];
  size_t n;

  cwk_path_set_style(CWK_STYLE_UNIX);

  n = cwk_path_change_extension("/test.txt", "md", buffer, sizeof(buffer));
  if (n != 8) {
    return EXIT_FAILURE;
  }

  if (strcmp("/test.md", buffer) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_check_without()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_has_extension("/my/path")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_check_empty()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_has_extension("")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_check_simple()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (!cwk_path_has_extension("/my/path.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_multiple()
{
  const char *path, *extension;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.abc.txt.tests";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 6) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".tests", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_last()
{
  const char *path, *extension;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 1) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_first()
{
  const char *path, *extension;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/.path";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 5) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".path", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_without()
{
  const char *path, *extension;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path";
  if (cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int extension_get_simple()
{
  const char *path, *extension;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt";
  if (!cwk_path_get_extension(path, &extension, &length)) {
    return EXIT_FAILURE;
  }

  if (length != 4) {
    return EXIT_FAILURE;
  }

  if (strncmp(extension, ".txt", length) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
