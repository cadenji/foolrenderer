---
title: cwk_path_join_multiple
description: Joins multiple paths together.
---

_(since v1.1.0)_  
Joins multiple paths together.

## Description
```c
size_t cwk_path_join_multiple(const char **paths, char *buffer,
  size_t buffer_size);
```
This function generates a new path by joining multiple paths together. It will remove double separators, and unlike [cwk_path_get_absolute]({{ site.baseurl }}{% link reference/cwk_path_get_absolute.md %}) it permits the use of multiple relative paths to combine. The last path of the submitted string array must be set to NULL. The result will be written to a buffer, which might be truncated if the buffer is not large enough to hold the full path. However, the truncated result will always be null-terminated. The returned value is the amount of characters which the resulting path would take if it was not truncated (excluding the null-terminating character).

## Parameters
 * **paths**: An array of paths which will be joined.
 * **buffer**: The buffer where the result will be written to.
 * **buffer_size**: The size of the result buffer.

## Return Value
Returns the total amount of characters of the full, combined path.

## Outcomes
See [cwk_path_join]({{ site.baseurl }}{% link reference/cwk_path_join.md %}) for examples how paths are joined together.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char buffer[FILENAME_MAX];
  char paths[3];

  paths[0] = "hello/there";
  paths[1] = "../world";
  paths[2] = NULL;
  cwk_path_join_multiple(paths, buffer, sizeof(buffer));
  printf("The combined path is: %s", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The combined path is: hello/world
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.1.0** | The function is introduced.                            |