---
title: cwk_path_get_dirname
description: Gets the dirname of a file path.
---

_(since v1.0.0)_  
Gets the dirname of a file path.

## Description
```c
void cwk_path_get_dirname(const char *path, size_t *length);
```

This function determines the dirname of a file path and returns the length up to which character is considered to be part of it. The dirname is the part of the path up to the last segment. For instance, `/var/` is the dirname of `/var/logs`. If no dirname is found, the length will be set to zero. The beginning of the dirname is always equal to the submitted path pointer.

## Parameters
 * **path**: The path which will be inspected.
 * **length**: The length of the dirname.

## Outcomes

| Path                   | Basename      |
|------------------------|---------------|
| ``/my/path.txt``       | ``/my/``      |
| ``/one/two/three.txt`` | ``/one/two/`` |
| ``../one/two.txt``     | ``../one/``   |
| `` ``                  | `` ``         |
| ``/my/path.txt/``      | ``/my/``      |
| ``/my/path.txt////``   | ``/my/``      |
| ``file_name``          | `` ``         |
| ``..``                 | `` ``         |
| ``.``                  | `` ``         |
| ``/``                  | `` ``         |

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
  cwk_path_get_dirname(path, &length);
  printf("The dirname is: '%.*s'", length, path);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The dirname is: '/my/'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |