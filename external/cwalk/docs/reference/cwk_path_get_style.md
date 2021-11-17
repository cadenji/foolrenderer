---
title: cwk_path_get_style
description: Gets the path style configuration..
---

_(since v1.0.0)_  
Gets the path style configuration..

## Description
```c
enum cwk_path_style cwk_path_get_style(void);
```

## Description
This function gets the style configuration which is currently used for the paths. This configuration determines how paths are parsed and generated.

 * ``CWK_STYLE_WINDOWS``: Use backslashes as a separator and volume for the root.
 * ``CWK_STYLE_UNIX``: Use slashes as a separator and a slash for the root.

## Return Value
Returns the current path style configuration.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if(cwk_path_get_style() == CWK_STYLE_WINDOWS) {
    printf("It's the WINDOWS style.");
  } else {
    printf("It's the UNIX style.");
  }
  
  return EXIT_SUCCESS;
}
```

Example Ouput:
```
It's the UNIX style.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |