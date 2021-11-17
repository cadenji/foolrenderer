---
title: cwk_path_get_root
description: Determines the root of a path.
---

_(since v1.0.0)_  
Determines the root of a path.

## Description
```c
void cwk_path_get_root(const char *path, size_t *length);
```
This function determines the root of a path by finding it's length.  The root comes before the first segment of the path. For example, ``C:\`` is the root of ``C:\folder\file.txt``. It always starts at the submitted path. If the path has no root, the length will be set to zero.

## Parameters
 * **path**: The path which will be inspected.
 * **length**: The output of the root length.

## Outcomes

| Style   | Path                     | Root                 |
|---------|--------------------------|----------------------|
| UNIX    | ``/test/``               | ``/``                |
| UNIX    | ``test.txt``             | `` ``                |
| UNIX    | ``C:\test.txt``          | `` ``                |
| UNIX    | ``\folder\``             | `` ``                |
| WINDOWS | ``/test.txt``            | ``/``                |
| WINDOWS | ``\test.txt``            | ``\``                |
| WINDOWS | ``C:\test.txt``          | ``C:\``              |
| WINDOWS | ``\\server\folder\data`` | ``\\server\folder\`` |
| WINDOWS | ``\\.\folder\data``      | ``\\.\``             |
| WINDOWS | ``\\?\folder\data``      | ``\\?\``             |
| WINDOWS | ``C:test.txt``           | ``C:``               |
| WINDOWS | ``..\hello\world.txt``   | `` ``                |

### Note
The style is automatically chosen during compile time, which is 
UNIX for macOS and linux and WINDOWS for windows. You can change the style
using [cwk_path_set_style]({{ site.baseurl }}{% link reference/cwk_path_set_style.md %}).

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  const char *path;
  size_t length;

  path = "/my/path.txt";
  cwk_path_get_root(path, &length);
  printf("The root is: '%.*s'", length, path);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The root is: '/'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.1.0** | The UNC path root now includes the shared folder name. |
| **v1.1.0** | Fixed false root detection on relative windows paths.  |
| **v1.0.0** | The function is introduced.                            |