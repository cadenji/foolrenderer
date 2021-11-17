---
title: Embedding
description: A guide on how to embed the cwalk path library for C/C++.
---


In order to embed **cwalk**, you will have to download it.
You can do so using git (or download it from [here](https://github.com/likle/cwalk/archive/stable.zip)).  

```bash
git clone -b stable git@github.com:likle/cwalk.git
```
**Note**: The *stable* branch points to the latest stable version. You should 
always use a stable version in production code.

## Using CMake to embed cwalk
If you are using CMake it is fairly easy to embed **cwalk**. 
This only requires two lines, you don't even have to specify the include directories.
The following example shows how to do so:
```cmake
# Some basics you will need in your cmake file.
cmake_minimum_required(VERSION 3.9.2)
project(example C)
add_executable(example_target main.c)

# Replace your_path_to_cwalk with the path to your cwalk copy. 
# This could be something like "${CMAKE_CURRENT_SOURCE_DIR}/lib/cwalk".
add_subdirectory(your_path_to_cwalk) 

# Replace example_target with the target name which requires cwalk.
# After this, there is no need to specify any include directories.
target_link_libraries(example_target cwalk)
```

After that, you should be able to use cwalk in your source code:
```c
#include <cwalk.h>
```

## Directly embed cwalk in your source
If you don't use CMake and would like to embed **cwalk** directly, you could 
just add the two files ``src/cwalk.c`` and ``ìnclude/cwalk.h`` to your project.
The folder containing ``cwalk.h`` has to be in your include directories 
([Visual Studio](https://docs.microsoft.com/en-us/cpp/ide/vcpp-directories-property-page?view=vs-2017), 
[Eclipse](https://help.eclipse.org/mars/index.jsp?topic=%2Forg.eclipse.cdt.doc.user%2Freference%2Fcdt_u_prop_general_pns_inc.htm), 
[gcc](https://www.rapidtables.com/code/linux/gcc/gcc-i.html),
[clang](https://clang.llvm.org/docs/ClangCommandLineReference.html#include-path-management)).