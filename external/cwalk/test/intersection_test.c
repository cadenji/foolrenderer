#include <cwalk.h>
#include <stdlib.h>

int intersection_skipped_end()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_get_intersection("/test/foo/har/../", "/test/foo/ho") != 9) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_relative_other()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_get_intersection("/test/foo/har", "/test/abc/../foo/bar") != 9) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_relative_base()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_get_intersection("/test/abc/../foo/bar", "/test/foo/har") !=
      16) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_one_root_only()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_get_intersection("C:/abc/test.txt", "C:/") != 3) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_same_roots()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_get_intersection("C:/abc/test.txt", "C:/def/test.txt") != 3) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_relative_absolute_mix()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_get_intersection("/test/abc.txt", "test/abc.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_unequal_roots()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_get_intersection("C:/test/abc.txt", "D:/test/abc.txt") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_empty()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_get_intersection("/", "") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_double_separator()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this///is/a//test", "/this//is/a///file") !=
      12) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_trailing_separator()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this/is/a/test", "/this/is/a/") != 10) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_truncated()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this/is/a/test", "/this/is/a") != 10) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int intersection_simple()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_get_intersection("/this/is/a/test", "/this/is/a/string") != 10) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
