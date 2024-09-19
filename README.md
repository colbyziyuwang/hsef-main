# HSEF

![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/rickvalenzano/8a7b862d017dfd60797a30b27a0d4f8d/raw/ubuntu_build.json)
![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/rickvalenzano/5177a442099ac9041097a8d823e0a0b2/raw/windows_build.json)
![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/rickvalenzano/ab67d012c4f0fabc61d1fcb141e49835/raw/macos_build.json)

Heuristic Search Experimental Framework (HSEF) is a framework for running
heuristic search experiments.

Below, you can find information about building the system. The instructions are for both debug mode
and release mode. Note that release mode is approximately 10x faster, but ignores asserts and uses
other optimizations. It is suggested you run your code first in debug mode.

### Building HSEF on Linux

- For Linux, one simply needs to have a C++ 17 compiler and CMake installed. These are generally
  standard on most linux distributions
- From the root directory, you can compile in debug mode using

```
cmake -B build/debug -DENABLE_DOXYGEN=OFF -DBUILD_VISUALIZER=OFF -DENABLE_TESTING=OFF -D CMAKE_BUILD_TYPE=Debug
cmake --build ./build/debug
```

- You can compile in release mode using

```
cmake -B build/release -DENABLE_DOXYGEN=OFF -DBUILD_VISUALIZER=OFF -DENABLE_TESTING=OFF -D CMAKE_BUILD_TYPE=Release
cmake --build ./build/release
```
- Note that the -DENABLE_DOXYGEN part can be ignored if you have doxygen installed. In that case,
  the doxygen documentation will also be generated
- The -DENABLE_TESTING part turns the compiling of the unit tests off. This should actually work on
  Linux
  but will likely increase compilation time significantly
- You might be able to speed up compilation if you add `-j 2` to the second command, as that uses
  multiple
  2 threads for compiling

For release mode,

### Building HSEF on MacOS

- This is also straightforward. Again, one just needs a C++ compiler and CMake.
- To install the clang C++ compiler, just install XCode from the App store
- For CMake, you can use homebrew, but check for additional instructions if you are on M1 mac
- The rest of the information for Linux applies here

### Building HSEF on Windows

- Again, you need a C++ compiler and CMake, but it is a bit more complicated here. Having said that,
  generally people have eventually figured it out
- If you want to use Visual Studio (NOT VSCode), you can try the instructions in
  VISUAL_STUDIO_README.md though those haven't been tested recently
- Based on what I've heard from others (this is a bit of buyer beware), you can also install it by
  doiung the following
  - install MinGW . This can be done using https://www.msys2.org
  - Using the UCRT64 environment, you can then install gcc and make
    using ``pacman -S mingw-w64-ucrt-x86_64-toolchain``
  - Then install cmake using ``pacman -S mingw-w64-ucrt-x86_64-cmake``
  - Add MinGW to your path in the environment variables: ``C:\msys64\ucrt64\bin``
  - Then use the following CMake commands to compile in debug mode

```
cmake -B build/debug -G “MinGW Makefiles” -DENABLE_DOXYGEN=OFF -DBUILD_VISUALIZER=OFF -DENABLE_TESTING=OFF
cmake --build build/debug --config Debug
```

- For release mode, simply change `build/debug` in both commands to `build/release`, and use `--config Release` in the second command instead of `--config Debug`
- If you have issues, try ignoring the `-G “MinGW Makefiles”` part of the first command as that may
  not be necessary for your system

## Running the Tutorials

If you look in the folder ``tutorials``, you will find tutorials that you should go through.
We encourage you to simultaneously look through the .cpp and run the associated binary for that .cpp
file.
Assuming you compiled in debug mode, you should be able to find the binaries in ``build/debug/tutorials`` or ``build/tutorials``
or ``build/Debug/tutorials`` depending on your system
