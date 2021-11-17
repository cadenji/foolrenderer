---
title: cwk_path_get_segment_type
description: Gets the type of the submitted path segment.
---

_(since v1.0.0)_  
Gets the type of the submitted path segment.

## Description
```c
enum cwk_segment_type cwk_path_get_segment_type(const struct cwk_segment *segment);
```

This function inspects the contents of the segment and determines the type of it. Currently, there are three types ``CWK_NORMAL``, ``CWK_CURRENT`` and ``CWK_BACK``. A ``CWK_NORMAL`` segment is a normal folder or file entry. A ``CWK_CURRENT`` is a "./" and a ``CWK_BACK`` a "../" segment.

## Parameters
 * **segment**: The segment which will be inspected.

## Return Value
Returns the type of the segment.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  struct cwk_segment segment;

  if(!cwk_path_get_first_segment("/../funny/test/path.txt", &segment)) {
    printf("Path doesn't have any segments.");
    return EXIT_FAILURE;
  }
  
  if(cwk_path_get_segment_type(&segment) == CWK_BACK) {
    printf("The segment type is CWK_BACK.");
  }
  
  return EXIT_SUCCESS;
}
```

Ouput:
```
The segment type is CWK_BACK.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |