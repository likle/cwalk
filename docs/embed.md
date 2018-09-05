---
title: Embedding
description: A guide on how to embed the cwalk path library for C/C++.
---

Before you embed `cwalk` you might have to [build]({{ site.baseurl }}{% link build.md %}) it first.

## Using CMake
If you are using CMake it is fairly easy to embed ``cwalk``. 
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