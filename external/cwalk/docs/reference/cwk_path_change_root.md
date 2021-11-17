---
title: cwk_path_change_root
description: Changes the root of a path.
---

_(since v1.2.0)_  
Changes the root of a path.

## Description
```c
size_t cwk_path_change_root(const char *path, const char *new_root,
  char *buffer, size_t buffer_size);
```

This function changes the root of a path. It does not normalize the result.
The result will be written to a buffer, which might be truncated if the
buffer is not large enough to hold the full path. However, the truncated
result will always be null-terminated. The returned value is the amount of
characters which the resulting path would take if it was not truncated
(excluding the null-terminating character).

**Note:** This function does not normalize the resulting path. You can use 
**[cwk_path_normalize]({{ site.baseurl }}{% link reference/cwk_path_normalize.md %})**
to do so. The new root may contain separators which will introduce new segments. 
If the submitted path does not have any root, the new root will be prepended to 
the path.

## Parameters
 * **path**: The original path which will get a new root.
 * **new_root**: The new root which will be placed in the path.
 * **buffer**: The output buffer where the result is written to.
 * **buffer_size**: The size of the output buffer where the result is written 
 to.

## Return Value
Returns the total amount of characters of the new path.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char buffer[FILENAME_MAX];

  cwk_path_set_style(CWK_STYLE_WINDOWS);

  cwk_path_change_root("C:\\test.txt", "D:\\", buffer,
    sizeof(buffer));

  printf("The new path: '%s'", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The new path: 'D:\another.txt'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.2.0** | The function is introduced.                            |