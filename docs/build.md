---
title: Building
description: A guide on how to build the cwalk path library for C/C++.
---

In order to build the source, you will have to download it. You can do so using git (or download it from [here](https://github.com/likle/cwalk/archive/stable.zip)).
```bash
git clone -b stable git@github.com:likle/cwalk.git
```

**Note**: The *stable* branch points to the latest stable version. You should
always use a stable version in production code.

CMake can be used on any platform with any C11-standard C compiler to build CWalk.
For example, GCC and Clang are widely available compilers.

```sh
cmake -Bbuild
cmake --build build
```

This results in binary library "build/libcwalk.a".

## Using Visual Studio

CWalk works with Visual Studio and other C compilers on Windows.
If using Visual Studio IDE, open the project using ``File -> Open -> CMake...``.
You can use Visual Studio to compile the source and debug the code.
Make sure Visual Studio CMake and C/C++ features are enabled.

## Using Linux

You will need [CMake](https://cmake.org/download/) and a C compiler such as GCC or Clang.
For example, on Ubuntu:

```sh
sudo apt-get install gcc cmake make
```

## Using MacOS

You will need [CMake](https://cmake.org/download/) and C compiler such as GCC or Clang.
Using [Homebrew](https://brew.sh):

```sh
brew install cmake gcc
```
# Running Tests

In order to run tests, cwalk needs to be built with tests enabled via ``ENABLE_TESTS`` flag:

```sh
cmake -Bbuild -DENABLE_TESTS=1
```

After building **cwalk** you can optionally run tests to ensure everything is fine:

```sh
ctest --test-dir build
```

To install CWalk under a prefix directory (e.g. "/opt/cwalk"):

```sh
cmake -Bbuild --install-prefix /opt/cwalk
cmake --build build
cmake --install build
```

That's it!

You can even specify which tests to execute by optionally specifying the category and test name:
```bash
# ./cwalktest [category] [test]
./cwalktest normalize mixed
```

or from CMake CTest, to run a category of tests:

```sh
ctest --test-dir build -L category
```

or to run a test by name

```sh
ctest --test-dir build -R test_name
```
