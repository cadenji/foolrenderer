#include <cwalk.h>
#include <stdlib.h>

int is_absolute_relative_windows()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_absolute("..\\hello\\world.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_relative_drive()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (cwk_path_is_absolute("C:test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_device_question_mark()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_absolute("\\\\?\\mydevice\\test")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_device_dot()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_is_absolute("\\\\.\\mydevice\\test");

  if (!cwk_path_is_absolute("\\\\.\\mydevice\\test")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_device_unc()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_absolute("\\\\.\\UNC\\LOCALHOST\\c$\\temp\\test-file.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_unc()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_absolute("\\\\server\\folder\\data")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_absolute_drive()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_absolute("C:\\test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_unix_drive()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_is_absolute("C:\\test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_unix_backslash()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_is_absolute("\\folder\\")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_windows_slash()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_absolute("/test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_windows_backslash()
{
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_is_absolute("\\test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_relative()
{
  cwk_path_set_style(CWK_STYLE_UNIX);

  if (cwk_path_is_absolute("test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int is_absolute_absolute()
{
  cwk_path_set_style(CWK_STYLE_UNIX);
  if (!cwk_path_is_absolute("/test.txt")) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
