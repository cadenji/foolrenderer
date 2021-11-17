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

## Using Windows
Visual Studio 2017 is recommended, then you can just open the source using ``File -> Open -> CMake...``. You can use Visual Studio to compile the source and debug the code. Make sure you have the CMake and C/C++ features enabled.

## Using Ubuntu
You will need [CMake](https://cmake.org/download/) and either gcc or clang installed. On Ubuntu you can use the following to compile **cwalk**:
```bash
sudo apt-get install build-essential cmake
mkdir cwalk/build
cd cwalk/build
cmake ..
make
```

## Using MacOS
You will need [CMake](https://cmake.org/download/) and either gcc or clang installed. On MacOS you can use the following to compile **cwalk**:
```
brew install cmake gcc
mkdir cwalk/build
cd cwalk/build
cmake ..
make
```
# Running Tests
After building **cwalk** you can run tests to ensure everything is fine. In order to do that, make sure that you are in the build folder and then execute the test program:

```bash
./cwalktest
```

That's it!

You can even specify which tests to execute by optionally specifying the category and test name:
```bash
# ./cwalktest [category] [test]
./cwalktest normalize mixed
```
