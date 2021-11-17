---
title: cwk_path_get_first_segment
description: Gets the first segment of a path.
---

_(since v1.0.0)_  
Gets the first segment of a path.

## Description
```c
bool cwk_path_get_first_segment(const char *path, struct cwk_segment *segment);
```

## Description
This function finds the first segment of a path. The position of the segment is set to the first character after the separator, and the length counts all characters until the next separator (excluding the separator).

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

  if(!cwk_path_get_first_segment("/my/path.txt", &segment)) {
    printf("Path doesn't have any segments.");
  }

  printf("Segment length is '%zu'.\n", segment.size);
  printf("The segment is '%.*s'.", (int)segment.size, segment.begin);

  return EXIT_SUCCESS;
}
```

Ouput:
```
Segment length is '2'.
The segment is 'my'.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
