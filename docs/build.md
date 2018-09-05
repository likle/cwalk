---
title: Building
description: A guide on how to build the cwalk path library for C/C++.
---

In order to get the source, you will have to download it. You can do so using git (or download it from [here](https://github.com/likle/cwalk/archive/stable.zip)).
```bash
git clone -b stable git@github.com:likle/cwalk.git
```

## Using Windows
Visual Studio 2017 is recommended, then you can just open the source using ``File -> Open -> CMake...``. You can use Visual Studio to compile the source and debug the code. Make sure you have the CMake and C/C++ features enabled.

## Using Ubuntu
You will need [CMake](https://cmake.org/download/) and either gcc or clang installed. On Ubuntu you can use the following to compile ``cwalk``:
```bash
sudo apt-get install build-essential cmake
mkdir cwalk/build
cd cwalk/build
cmake ..
make
```

## Using MacOS
You will need [CMake](https://cmake.org/download/) and either gcc or clang installed. On MacOS you can use the following to compile ``cwalk``:
```
brew install cmake gcc
mkdir cwalk/build
cd cwalk/build
cmake ..
make
```
# Running Tests

After [building](https://github.com/likle/cwalk/wiki/Building) ``cwalk`` you can run tests to ensure everything runs fine. In order to do that, make sure you are in the build folder and then use either CTest or the native test program.

## Using CTest
To run CTest run the following in the build folder:
```bash
make test
```

That's it!

## Using the native test program
CTest is slightly slower than the native test program since it starts a process for each test.
If you want to run the native test program you can do so by running the following in the build folder:
```bash
./cwalktest
```

You can even specify which tests to execute by optionally specifying the category and test name:
```bash
# ./cwalktest [category] [test]
./cwalktest normalize mixed
```
