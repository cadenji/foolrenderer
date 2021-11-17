#include <cwalk.h>
#include <stdlib.h>

int is_relative_relative_windows()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_relative("..\\hello\\world.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_relative_drive()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_relative("C:test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_device_question_mark()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_relative("\\\\?\\mydevice\\test")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_device_dot()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_is_absolute("\\\\.\\mydevice\\test");

  if (cwk_path_is_relative("\\\\.\\mydevice\\test")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_device_unc()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_relative("\\\\.\\UNC\\LOCALHOST\\c$\\temp\\test-file.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_unc()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_relative("\\\\server\\folder\\data")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_absolute_drive()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_relative("C:\\test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_unix_drive()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (!cwk_path_is_relative("C:\\test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_unix_backslash()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (!cwk_path_is_relative("\\folder\\")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_windows_slash()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_relative("/test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_windows_backslash()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_relative("\\test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_relative()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (!cwk_path_is_relative("test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_relative_absolute()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (cwk_path_is_relative("/test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
