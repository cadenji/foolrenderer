---
title: cwk_path_get_extension
description: Gets the extension of a file path.
---

_(since v1.0.0)_  
Gets the extension of a file path.

## Description
```c
bool cwk_path_get_extension(const char *path, const char **extension, size_t *length);
```

This function extracts the extension portion of a file path. A pointer to the beginning of the extension will be returned through the extension parameter if an extension is found and true is returned. This pointer will be positioned on the dot. The length of the extension name will be returned through the length parameter. If no extension is found both parameters won't be touched and false will be returned.

## Parameters
 * **path**: The path which will be inspected.
 * **extension**: The output of the extension pointer.
 * **length**: The output of the length of the extension.

## Outcomes

| Path                       | Result     |
|----------------------------|------------|
| ``/my/path.txt``           | ``.txt``   |
| ``/my/path``               | `` ``      |
| ``/my/.path``              | ``.path``  |
| ``/my/path.``              | ``.``      |
| ``/my/path.abc.txt.tests`` | ``.tests`` |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  const char *path, *extension;
  size_t length;

  path = "/my/path.txt";
  cwk_path_get_extension(path, &extension, &length);
  printf("The extension is: '%.*s'", length, path);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The extension is: '.txt'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |