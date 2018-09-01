[![Travis Build](https://img.shields.io/travis/likle/cwalk/master.svg?maxAge=2592000&label=Linux%20%26%20MacOS)](https://travis-ci.org/likle/cwalk)
[![Appveyor Build](https://img.shields.io/appveyor/ci/likle/cwalk/master.svg?label=Windows)](https://travis-ci.org/likle/cwalk) 
[![codecov](https://img.shields.io/codecov/c/github/likle/cwalk/master.svg?label=Coverage)](https://codecov.io/gh/likle/cwalk)
[![Language Grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/likle/cwalk.svg?label=Code%20Quality)](https://lgtm.com/projects/g/likle/cwalk/context:cpp)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg?label=License)](https://github.com/likle/cwalk/blob/master/LICENSE)

# libcwalk - path library for C/C++
This library is intended to be a lighweight C path manipulation library. 
It is currently compiled and tested under **Windows**, **MacOS** and **Linux**. 
It supports UNIX and Windows path styles on all platforms.

## WARNING
This is a work in progress and not yet ready to use.

## Features
Please have a look at the **[reference](https://github.com/likle/cwalk/wiki/Reference)** for detailed information.
Some features this library includes:

 * **cross-platform** on windows, linux and macOS
 * **simple [interface](https://github.com/likle/cwalk/wiki/Reference)** - just one header 
 * **[combine](https://github.com/likle/cwalk/wiki/cwk_path_join) paths** together
 * **[basename](https://github.com/likle/cwalk/wiki/cwk_path_get_basename), [dirname](https://github.com/likle/cwalk/wiki/cwk_path_get_dirname) and [extension](https://github.com/likle/cwalk/wiki/cwk_path_get_extension)** parsing
 * **[normalize](https://github.com/likle/cwalk/wiki/cwk_path_normalize) and cleanup** paths
 * **[resolve](https://github.com/likle/cwalk/wiki/cwk_path_get_absolute_path) and [generate](https://github.com/likle/cwalk/wiki/cwk_path_get_relative_path) relative** paths
 * **[iterate](https://github.com/likle/cwalk/wiki/cwk_path_get_next_segment) segments** of the path
 
 ## Building
 **[Building](https://github.com/likle/cwalk/wiki/Building)**, 
 **[embedding](https://github.com/likle/cwalk/wiki/Embedding)** and 
 **[testing](https://github.com/likle/cwalk/wiki/Testing)** 
 instructions are available in the documentation (it's very easy).
 
 ## Docs
 All the documentation is available in the **[wiki](https://github.com/likle/cwalk/wiki)** of this repository.