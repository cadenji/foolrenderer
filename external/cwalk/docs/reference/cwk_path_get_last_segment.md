---
title: cwk_path_get_last_segment
description: Gets the last segment of the path.
---

_(since v1.0.0)_  
Gets the last segment of the path.

## Description
```c
bool cwk_path_get_last_segment(const char *path, struct cwk_segment *segment);
```

This function gets the last segment of a path. This function may return false if the path doesn't contain any segments, in which case the submitted segment parameter is not modified. The position of the segment is set to the first character after the separator, and the length counts all characters until the end of the path (excluding the separator).

## Parameters
 * **path**: The path which will be inspected.
 * **segment**: The segment which will be extracted.

## Return Value
Returns ``true`` if there is a segment or ``false`` if there is none.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  struct cwk_segment segment;

  if(!cwk_path_get_last_segment("/my/test/path.txt", &segment)) {
    printf("Path doesn't have any segments.");
  }

  printf("Segment length is '%zu'.\n", segment.size);
  printf("The segment is '%.*s'.", (int)segment.size, segment.begin);

  return EXIT_SUCCESS;
}
```

Ouput:
```
Segment length is '8'.
The segment is 'path.txt'.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
