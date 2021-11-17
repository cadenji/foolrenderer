---
title: cwk_path_set_style
description: Configures which path style is used.
---

_(since v1.0.0)_  
Configures which path style is used.

## Description
```c
void cwk_path_set_style(enum cwk_path_style style);
```

## Description
This function configures which path style is used. A call to this function is only required if a non-native behaviour is required. The style defaults to ``CWK_STYLE_WINDOWS`` on windows builds and to ``CWK_STYLE_UNIX`` otherwise. The following styles are currently supported.

 * ``CWK_STYLE_WINDOWS``: Use backslashes as a separator and volume for the root.
 * ``CWK_STYLE_UNIX``: Use slashes as a separator and a slash for the root.

## Parameters
 * **style**: The style which will be used from now on.

## Example
```c
#include <cwalk.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  size_t length;
  
  cwk_path_set_style(CWK_STYLE_WINDOWS);
  cwk_path_get_root("C:\\test.txt", &length);
  printf("The root length is '%zu'.", length);
  return EXIT_SUCCESS;
}
```

Ouput:
```
The root length is '3'.
```

## Changelog

| Version    | Description                                            |
|------------|--------------------------------------------------------|
| **v1.0.0** | The function is introduced.                            |