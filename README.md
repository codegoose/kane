# Project Kane

This is a work-in-progress 2D sidescrolling strategy and resource management game.

<p align="center">
 <img width="605" height="216" src="https://github.com/codegoose/kane/blob/master/snapshots/progress-snap-20210109.gif?raw=true">
</p>

This is a short guide on how to build this software on Windows. You'll need MSYS2. I'll walk you through it:

## Building

### Installing MSYS2

1. Use [Scoop](https://scoop.sh/) to install ```msys2```.
2. Add ```scoop\apps\msys2\current\mingw64\bin``` and ```scoop\apps\msys2\current\usr\bin``` to your path variable in that order.

### Installing Required Packages

You'll now have ```pacman``` available via the command line. Perform an initial update by running ```pacman -Syyu```.

After that's done, install the packages listed below. (```pacman -S package names go here```)

### Installing Development Packages

```
mingw-w64-x86_64-clang mingw-w64-x86_64-lld mingw-w64-x86_64-lldb mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

This amounts to using Clang to compile and LLD to link. The biggest benefit here is that LLD is **way** faster at linking that other tools. And reduced link times from 12 seconds to under a second on my machine.

### Installing Dependency Packages

```
mingw-w64-x86_64-spdlog mingw-w64-x86_64-fmt mingw-w64-x86_64-glew mingw-w64-x86_64-glm mingw-w64-x86_64-glfw mingw-w64-x86_64-nlohmann-json mingw-w64-x86_64-freetype mingw-w64-x86_64-sfml
```

### Compiling and Linking

Make a build directory within the project folder, ```cd``` into it and run:

```
cmake -G Ninja .. -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld
```

That's telling ```cmake``` that we wanna compile with Clang and link with LLD and that ```ninja``` should manage that process.

Finally, build it by running ```ninja``` or ```cmake --build .``` within that folder.
