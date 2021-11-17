---
title: cwk_path_get_relative
description: Generates a relative path based on a base.
---

_(since v1.0.0)_  
Generates a relative path based on a base.

## Description
```c
size_t cwk_path_get_relative(const char *base_directory, const char *path,
  char *buffer, size_t buffer_size);
```

This function generates a relative path based on a base path and another path. It determines how to get to the submitted path, starting from the base directory. The result will be written to a buffer, which might be truncated if the buffer is not large enough to hold the full path. However, the truncated result will always be null-terminated. The returned value is the amount of characters which the resulting path would take if it was not truncated (excluding the null-terminating character).

## Parameters
 * **base_directory**: The base path from which the relative path will start.
 * **path**: The target path where the relative path will point to.
 * **buffer**: The buffer where the result will be written to.
 * **buffer_size**: The size of the result buffer.

## Return Value
Returns the total amount of characters of the full path.

## Outcomes

| Style   | Base                     | Path                     | Result          |
|---------|--------------------------|--------------------------|-----------------|
| UNIX    | ``/../../``              | ``/../../``              | ``.``           |
| UNIX    | ``/path/same``           | ``/path/not_same/ho/..`` | ``../not_same`` |
| UNIX    | ``/path/not_same/ho/..`` | ``/path/same``           | ``../same``     |
| UNIX    | ``/path/same``           | ``/path/same/ho/..``     | ``.``           |
| UNIX    | ``/path/same/ho/..``     | ``/path/same``           | ``.``           |
| UNIX    | ``/path/same``           | ``/path/same``           | ``.``           |
| UNIX    | ``/path/long/one``       | ``/path/long/one/two``   | ``two``         |
| UNIX    | ``/path/long/one/two``   | ``/path/long/one``       | ``..``          |
| UNIX    | ``./this/is/path_one``   | ``./this/is/path_two``   | ``../path_two`` |
| UNIX    | ``/this/is/path_one``    | ``/this/is/path_two``    | ``../path_two`` |
| WINDOWS | ``C:/path/same``         | ``D:/path/same``         | `` ``           |

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char buffer[FILENAME_MAX];
  
  cwk_path_get_relative("/hello/there/", "/hello/world", buffer, sizeof(buffer));
  printf("The relative path is: %s", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The relative path is: ../world
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.2.5** | Fixed calls with path without segments.                |
| **v1.2.5** | Fixed calls with unequal roots in paths.               |
| **v1.0.0** | The function is introduced.                            |
