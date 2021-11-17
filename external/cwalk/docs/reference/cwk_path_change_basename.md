---
title: cwk_path_change_basename
description: Changes the basename of a file path.
---

_(since v1.2.0)_  
Changes the basename of a file path.

## Description
```c
size_t cwk_path_change_basename(const char *path, const char *new_basename,
  char *buffer, size_t buffer_size);
```

This function changes the basename of a file path. This function will not
write out more than the specified buffer can contain. However, the generated
string is always null-terminated - even if not the whole path is written out.
The function returns the total number of characters the complete buffer would
have, even if it was not written out completely. The path may be the same
memory address as the buffer.

**Note:** This function does not normalize the resulting path. You can use 
**[cwk_path_normalize]({{ site.baseurl }}{% link reference/cwk_path_normalize.md %})**
to do so. Separators before and after the submitted basename will be trimmed, 
but not removed from the source path. The value may contain separators which 
will introduce new segments. If the submitted path does not have any segments, 
the basename will be appended as a new segment.

## Parameters
 * **path**: The original path which will be used for the modified path.
 * **new_basename**: The new basename which will replace the old one.
 * **buffer**: The buffer where the changed path will be written to.
 * **buffer_size**: The size of the result buffer where the changed path is written to.

## Return Value
Returns the size which the complete new path would have if it was not truncated.

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

  cwk_path_change_basename("C:\\test.txt", "another.txt", buffer,
    sizeof(buffer));

  printf("The new path: '%s'", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The new path: 'C:\another.txt'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.2.0** | The function is introduced.                            |