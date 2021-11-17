#include <cwalk.h>
#include <memory.h>
#include <stdlib.h>

int dirname_relative()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "../one/two.txt";
  cwk_path_get_dirname(path, &length);

  if (length != 7) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_three_segments()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/one/two/three.txt";
  cwk_path_get_dirname(path, &length);

  if (length != 9) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_root()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_special_directories()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "..";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  path = ".";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_no_separators()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "file_name";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_trailing_separators()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt////";
  cwk_path_get_dirname(path, &length);

  if (length != 4) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_trailing_separator()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt/";
  cwk_path_get_dirname(path, &length);

  if (length != 4) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_empty()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "";
  cwk_path_get_dirname(path, &length);

  if (length != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dirname_simple()
{
  const char *path;
  size_t length;

  cwk_path_set_style(CWK_STYLE_UNIX);
  path = "/my/path.txt";
  cwk_path_get_dirname(path, &length);

  if (length != 4) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
