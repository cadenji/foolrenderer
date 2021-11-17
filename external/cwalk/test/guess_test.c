#include <cwalk.h>
#include <stdlib.h>

int guess_empty_string()
{
  if (cwk_path_guess_style("") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_unguessable()
{
  if (cwk_path_guess_style("myfile") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_extension()
{
  if (cwk_path_guess_style("myfile.txt") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style("/a/directory/myfile.txt") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_hidden_file()
{
  if (cwk_path_guess_style(".my_hidden_file") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style(".my_hidden_file.txt") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_unix_separator()
{
  if (cwk_path_guess_style("/directory/other") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style("/directory/other.txt") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_windows_separator()
{
  if (cwk_path_guess_style("\\directory\\other") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }
  if (cwk_path_guess_style("\\directory\\.other") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_unix_root()
{
  if (cwk_path_guess_style("/directory") != CWK_STYLE_UNIX) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int guess_windows_root()
{
  if (cwk_path_guess_style("C:\\test") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style("C:/test") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style("C:test") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style("C:/.test") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  if (cwk_path_guess_style("C:/folder/.test") != CWK_STYLE_WINDOWS) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
