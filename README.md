# Manta Engine [![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com) [![forthebadge](https://forthebadge.com/images/badges/built-with-resentment.svg)](https://forthebadge.com)


The Manta engine is a one man project to build a basic game engine capable of making basic games with. It is in no way meant to compete with Unreal, Unity, Godot, and any other game engine developed by professionals. This project may be abandoned in the future depending on my motivation.

Parts of this software are inspired from features inside of well known game engines such as Id Tech 2 / Source, namely the console, on the surface they're functionally similar but I've extended the behaviour of my console implementation to fit my enigine's needs.

This software was written using NeoVim with many customizations and plugins to make it adequate for C/C++ and C# programming. Any formatting issues are the result of Vim and poor programming practices.

<!-- I am aware this table is a mess, just let it be for now, it isn't hurting anyone -->
### Supported Platforms

<!--Html here is necessary and is very messy!-->

| OS            | Compilers     | Architectures  | Build |
| :-----------  |:-------------:| :-----:        | :---: |
| <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/1200px-Tux.svg.png" width="32"/> Linux | GNU GCC, LLVM Clang | x86, amd64 | Master: ![Linux Build](https://github.com/zCubed3/Manta/actions/workflows/linuxbuild.yml/badge.svg?branch=master) <br/> Experimental: ![Linux Build](https://github.com/zCubed3/Manta/actions/workflows/linuxbuild.yml/badge.svg?branch=experimental) <br/>|

### Unsupported Platforms

| OS            | Compilers     | Architectures  | Planned support? |
| :-----------  |:-------------:| :-----:        | :---:    |
| <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/5f/Windows_logo_-_2012.svg/1200px-Windows_logo_-_2012.svg.png" width="32"/> Windows      | MinGW / Cygwin | x86, amd64 | Yes |
| <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/22/MacOS_logo_%282017%29.svg/512px-MacOS_logo_%282017%29.svg.png" width="32"/> MacOS         | N/A           | N/A            | No |
| <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/c/ca/IOS_logo.svg/512px-IOS_logo.svg.png" width="32"/> iOS           | N/A           | N/A            | No | 
| <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/d7/Android_robot.svg/511px-Android_robot.svg.png" width="32"/> Android       | N/A           | N/A            | No |  

(If I can get a working M1 Mac sometime in the future I will work on porting the engine to MacOS and iOS)

### Build instructions / requirements

**Disclaimer: If using Windows, please install Cygwin or any other Windows-POSIX compilation layer**

**Warning: If projects/engine_data isn't copied into the binary directory automatically, please do it manually since the engine will not function properly without it! (copy it and rename it to "data")**

#### Compilation requirements

1. CMake (3.19 or higher)
2. Compilers
    * GNU GCC or LLVM Clang
3. Libraries
    * GLEW
    * GLFW3

#### Compiling the code

1. Create the project files using CMake
    1. Run configure.sh and pick your compiler or run `./configure COMPILER_NAME "debug"/"release"` Ex: `./configure clang debug` would generate a debug build built with Clang
2. Compile the project
    1. After having ran `./configure` run `./build`, it'll run make and copy over engine content to the build

### Credits
1. Other projects used
    * GL Mathematics library, licensed under the MIT license, [g-truc/glm](https://github.com/g-truc/glm)
    * Dear ImGui, licensed under the MIT license, [ocornut/imgui](https://github.com/ocornut/imgui)
    * Mono, licensed under the MIT license and some portions under the BSD-3 Clause License, [mono/mono](https://github.com/mono/mono)

2. Resources
    * Utah Teapot model (used for testing shading)

3. Misc
   * For the badge [forthebadge](forthebadge.com) for the header images!<br/>
