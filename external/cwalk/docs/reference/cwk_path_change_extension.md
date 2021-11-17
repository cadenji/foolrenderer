---
title: cwk_path_change_extension
description: Changes the extension of a file path.
---

_(since v1.2.0)_  
Changes the extension of a file path.

## Description
```c
size_t cwk_path_change_extension(const char *path, const char *new_extension,
  char *buffer, size_t buffer_size);
```

This function changes the extension of a file name. The function will append
an extension if the basename does not have an extension, or use the extension
as a basename if the path does not have a basename. This function will not
write out more than the specified buffer can contain. However, the generated
string is always null-terminated - even if not the whole path is written out.
The function returns the total number of characters the complete buffer would
have, even if it was not written out completely. The path may be the same
memory address as the buffer.

**Note:** This function does not normalize the resulting path. You can use 
**[cwk_path_normalize]({{ site.baseurl }}{% link reference/cwk_path_normalize.md %})**
to do so.

**Note:** If the ``new_extension`` parameter starts with a dot, the first dot will
be ignored when the new extension is appended.

## Parameters
 * **path**: The path which will be used to make the change.
 * **new_extension**: The extension which will be placed within the new path.
 * **buffer**: The output buffer where the result will be written to.
 * **buffer_size**: The size of the output buffer where the result will be written to.

## Return Value
Returns the total size which the output would have if it was not truncated.

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

  cwk_path_change_extension("C:\\test.txt", "md", buffer,
    sizeof(buffer));

  printf("The new path: '%s'", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The new path: 'C:\test.md'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.2.0** | The function is introduced.                            |
