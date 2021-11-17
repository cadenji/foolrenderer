---
title: cwk_path_get_basename
description: Gets the basename of a file path.
---

_(since v1.0.0)_  
Gets the basename of a file path.

## Description
```c
void cwk_path_get_basename(const char *path, const char **basename,
  size_t *length);
```

This function gets the basename of a file path. The basename is the last segment of a path. For instance, ``logs`` is the basename of the path ``/var/logs``. A pointer to the beginning of the basename will be returned through the basename parameter. This pointer will be positioned on the first letter after the separator. The length of the file path will be returned through the length parameter. The length will be set to zero and the basename to NULL if there is no basename available.

## Parameters
 * **path**: The path which will be inspected.
 * **basename**: The output of the basename pointer.
 * **length**: The output of the length of the basename.

## Outcomes

| Path                 | Basename      |
|----------------------|---------------|
| ``/my/path.txt``     | ``path.txt``  |
| ``/my/path.txt/``    | ``path.txt``  |
| ``/my/path.txt////`` | ``path.txt``  |
| ``file_name``        | ``file_name`` |
| ``..``               | ``..``        |
| ``.``                | ``.``         |
| ``/``                | `` ``         |
| ``C:\path\test.txt`` | ``test.txt``  |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  const char *basename;
  size_t length;

  cwk_path_get_basename("/my/path.txt", &basename, &length);
  printf("The basename is: '%.*s'", length, basename);

  return EXIT_SUCCESS;
}
```
Ouput:
```
The basename is: 'path.txt'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |