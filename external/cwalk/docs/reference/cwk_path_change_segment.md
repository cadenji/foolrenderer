---
title: cwk_path_change_segment
description: Changes the content of a segment.
---

_(since v1.2.0)_  
Changes the content of a segment.

## Description
```c
size_t cwk_path_change_segment(struct cwk_segment *segment, const char *value,
  char *buffer, size_t buffer_size);
```

This function overrides the content of a segment to the submitted value and
outputs the whole new path to the submitted buffer. The result might require
less or more space than before if the new value length differs from the
original length. The output is truncated if the new path is larger than the
submitted buffer size, but it is always null-terminated. The source of the
segment and the submitted buffer may be the same.

**Note:** This function does not normalize the resulting path. You can use 
**[cwk_path_normalize]({{ site.baseurl }}{% link reference/cwk_path_normalize.md %})**
to do so. Separators before and after the value will be trimmed. The value may 
contain separators which will introduce new segments.

## Parameters
 * **segment**: The segment which will be modifier.
 * **value**: The new content of the segment.
 * **buffer**: The buffer where the modified path will be written to.
 * **buffer_size**: The size of the output buffer.

## Return Value
Returns the total size which would have been written if the output was not 
truncated.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  const char *path;
  char buffer[FILENAME_MAX];
  struct cwk_segment segment;

  path = "/this/cool/path/";
  if (!cwk_path_get_first_segment(path, &segment)) {
    return EXIT_FAILURE;
  }

  cwk_path_change_segment(&segment, "other", buffer, sizeof(buffer));

  printf("The new path: '%s'", buffer);

  return EXIT_SUCCESS;
}
```

Ouput:
```
The new path: '/other/cool/path/'
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.2.0** | The function is introduced.                            |
