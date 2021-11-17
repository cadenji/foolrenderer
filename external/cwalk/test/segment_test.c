#include <cwalk.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int segment_change_overlap()
{
  char buffer[FILENAME_MAX] = "C:\\this\\cool\\path\\";
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  if (!cwk_path_get_first_segment(buffer, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "longer_segment", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\longer_segment\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_change_with_separator()
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\cool\\path\\";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other\\fancy", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\other\\fancy\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_change_empty()
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\cool\\path\\";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_change_trim()
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\cool\\path\\";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "//other/\\\\", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\other\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_change_last()
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\cool\\path\\";
  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\cool\\other\\") != 0) {
    return EXIT_FAILURE;
  }

  path = "C:\\this\\cool\\path";
  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\cool\\other") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_change_first()
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\cool\\path\\";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\other\\cool\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  path = "this\\cool\\path\\";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other", buffer, sizeof(buffer));

  if (strcmp(buffer, "other\\cool\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_change_simple()
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\cool\\path\\";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other", buffer, sizeof(buffer));

  if (strcmp(buffer, "C:\\this\\other\\path\\") != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_back_with_root()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\this\\path";
  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "path", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "this", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_type()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/a/./../.folder/..folder";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_NORMAL) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_CURRENT) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_BACK) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_NORMAL) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_segment_type(&segment) != CWK_NORMAL) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_too_far_root()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "//now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_too_far()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "//now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_relative()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "now", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_absolute()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/now/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "now", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_relative_one_char_first()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "n/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "n", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }
  
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "t\\cool\\path\\";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "path", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "cool", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "t", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_previous_absolute_one_char_first()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/n/hello_world/abc/";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "n", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }
  
  cwk_path_set_style(CWK_STYLE_WINDOWS);

  path = "C:\\t\\cool\\path\\";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "path", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "cool", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "t", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_previous_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_next_too_far()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc/";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_next()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc/";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (!cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  if (cwk_path_get_next_segment(&segment)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_last()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  path = "hello_world/abc";

  if (!cwk_path_get_last_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "abc", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int segment_first()
{
  const char *path;
  struct cwk_segment segment;

  cwk_path_set_style(CWK_STYLE_UNIX);

  path = "/hello_world/abc";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  path = "hello_world/abc";

  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  if (strncmp(segment.begin, "hello_world", segment.size) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
