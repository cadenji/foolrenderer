---
title: cwk_path_join
description: Joins two paths together.
---

_(since v1.0.0)_  
Joins two paths together.

## Description
```c
size_t cwk_path_join(const char *path_a, const char *path_b, char *buffer,
  size_t buffer_size);
```

This function generates a new path by combining the two submitted paths. It will remove double separators, and unlike [cwk_path_get_absolute]({{ site.baseurl }}{% link reference/cwk_path_get_absolute.md %}) it permits the use of two relative paths to combine. The result will be written to a buffer, which might be truncated if the buffer is not large enough to hold the full path. However, the truncated result will always be null-terminated. The returned value is the amount of characters which the resulting path would take if it was not truncated (excluding the null-terminating character).

## Parameters
 * **path_a**: The first path which comes first.
 * **path_b**: The second path which comes after the first.
 * **buffer**: The buffer where the result will be written to.
 * **buffer_size**: The size of the result buffer.

## Return Value
Returns the total amount of characters of the full, combined path.

## Outcomes

| Style   | Path A                | Path B                  | Result                                 |
|---------|-----------------------|-------------------------|----------------------------------------|
| UNIX    | ``hello/there``       | ``../world``            | ``hello/world``                        |
| UNIX    | ``/first``            | ``/second``             | ``/first/second``                      |
| UNIX    | ``hello``             | ``..``                  | ``.``                                  |
| UNIX    | ``hello/there``       | ``..``                  | ``hello``                              |
| UNIX    | ``hello``             | ``there``               | ``hello/there``                        |
| WINDOWS | ``this\``             | ``C:\..\..\is\a\test\`` | ``is\a\test``                          |
| WINDOWS | ``C:\this\path``      | ``C:\is\a\test\``       | ``C:\this\path\C:\is\a\test``          |   
| WINDOWS | ``C:\this\path``      | ``C:\..\is\a\test\``    | ``C:\this\path\is\a\test``             |
| WINDOWS | ``\\s1\unc\path``     | ``\\s2\unc\pa``         | ``\\s1\unc\pa\s2\unc\path``            |

### Style
The style is automatically chosen during compile time, which is 
UNIX for macOS and linux and WINDOWS for windows. You can change the style
using [cwk_path_join]({{ site.baseurl }}{% link reference/cwk_path_join.md %}).

### Result
The **path_b** parameter will always be treated as a relative path, so even if 
a driver letter is submitted on a windows style path, it will be treated as a 
folder.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char buffer[FILENAME_MAX];
  
  cwk_path_join("hello/there", "../world", buffer, sizeof(buffer));
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
| **v1.0.0** | The function is introduced.                            |
