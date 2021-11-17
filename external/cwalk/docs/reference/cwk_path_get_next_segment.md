---
title: cwk_path_get_next_segment
description: Advances to the next segment.
---

_(since v1.0.0)_  
Advances to the next segment.

## Description
```c
bool cwk_path_get_next_segment(struct cwk_segment *segment);
```

This function advances the current segment to the next segment. If there are no more segments left, the submitted segment structure will stay unchanged and false is returned. The segment should be either initialized using **[cwk_path_get_first_segment]({{ site.baseurl }}{% link reference/cwk_path_get_first_segment.md %})** or **[cwk_path_get_last_segment]({{ site.baseurl }}{% link reference/cwk_path_get_last_segment.md %})**.

## Parameters
 * **segment**: The current segment which will be advanced to the next one.

## Return Value
Returns ``true`` if another segment was found or ``false`` otherwise.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  struct cwk_segment segment;

  if(!cwk_path_get_first_segment("/my/funny/test/path.txt", &segment)) {
    printf("Path doesn't have any segments.");
    return EXIT_FAILURE;
  }
  
  do {
    printf("Current segment is '%.*s'.\n", (int)segment.size, segment.begin);
  } while(cwk_path_get_next_segment(&segment));
  
  return EXIT_SUCCESS;
}
```

Ouput:
```
Current segment is 'my'.
Current segment is 'funny'.
Current segment is 'test'.
Current segment is 'path.txt'.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |
