---
title: cwk_path_get_absolute
description: Generates an absolute path based on a base.
---

_(since v1.0.0)_  
Generates an absolute path based on a base.

## Description
```c
size_t cwk_path_get_absolute(const char *base, const char *path, char *buffer,
  size_t buffer_size);
```

This function generates an absolute path based on a base path and another path. It is guaranteed to return an absolute path. If the second submitted path is absolute, it will override the base path. The result will be written to a buffer, which might be truncated if the buffer is not large enough to hold the full path. However, the truncated result will always be null-terminated. The returned value is the amount of characters which the resulting path would take if it was not truncated (excluding the null-terminating character).

## Parameters
 * **base**: The absolute base path on which the relative path will be applied.
 * **path**: The relative path which will be applied on the base path.
 * **buffer**: The buffer where the result will be written to.
 * **buffer_size**: The size of the result buffer.

## Return Value
Returns the total amount of characters of the new absolute path.

## Outcomes

| Base                 | Path                | Result                |
|----------------------|---------------------|-----------------------|
| ``/hello/there``     | ``../../../../../`` | ``/``                 |
| ``/hello//../there`` | ``test//thing``     | ``/there/test/thing`` |
| ``hello/there``      | ``/test``           | ``/test``             |
| ``hello/there``      | ``test``            | ``/hello/there/test`` |
| ``/hello/there``     | ``/test``           | ``/test``             |
| ``/hello/there``     | ``..``              | ``/hello``            |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char buffer[FILENAME_MAX];
  
  cwk_path_get_absolute("/hello/there", "./world", buffer, sizeof(buffer));
  printf("The absolute path is: %s", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The absolute path is: /hello/there/world
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.2.4** | Fix for relative base path fallback on windows.        |
| **v1.2.3** | Fix for path generation when reusing buffers.          |
| **v1.0.0** | The function is introduced.                            |
