---
title: cwk_path_normalize
description: Creates a normalized version of the path.
---

_(since v1.0.0)_  
Creates a normalized version of the path.

## Description
```c
size_t cwk_path_normalize(const char *path, char *buffer, size_t buffer_size);
```

This function creates a normalized version of the path within the specified buffer. This function will not write out more than the specified buffer can contain. However, the generated string is always null-terminated - even if not the whole path is written out. The function returns the total number of characters the complete buffer would have, even if it was not written out completely. The path may be the same memory address as the buffer.

The following will be true for the normalized path:
 * "../" will be resolved.
 * "./" will be removed.
 * double separators will be fixed with a single separator.
 * separator suffixes will be removed.

## Parameters
 * **path**: The path which will be normalized.
 * **buffer**: The buffer where the new path is written to.
 * **buffer_size**: The size of the buffer.

## Return Value
The size which the complete normalized path has if it was not truncated.

## Outcomes

| Input                               | Output           |
|-------------------------------------|------------------|
| `/var`                              | `/var`           |
| `/var/logs/test/../../`             | `/var`           |
| `/var/logs/test/../../../../../../` | `/`              |
| `rel/../../`                        | `..`             |
| `/var////logs//test/`               | `/var/logs/test` |
| `/var/././././`                     | `/var`           |
| `/var/./logs/.//test/..//..//////`  | `/var`           |


## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char result[FILENAME_MAX];

  // The following function cleans up the input path and writes it
  // to the result buffer.
  cwk_path_normalize("/var/log/weird/////path/.././..///", result,
    sizeof(result));
  
  printf("%s\n", result);
  return EXIT_SUCCESS;
}
```

Output:
```
/var/log
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |