# Manta Engine

[![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-resentment.svg)](https://forthebadge.com) 

This is a small passion project that isn't intended to be ground breaking in any way. I am still learning better programming practices and have only been seriously using C/C++ for about a year now, any bugs and issues with this software are par for the course considering that it was written by a 16 year old...

This software was written using NeoVim with many customizations to make it adequate for C/C++ programming. Any formatting issues are the result of Vim.

<!-- I am aware this table is a mess, just let it be for now, it isn't hurting anyone -->

### Supported Platforms

<!--Html here is necessary and is very messy!-->
<a><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/1200px-Tux.svg.png" width="32"/>
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/5f/Windows_logo_-_2012.svg/1200px-Windows_logo_-_2012.svg.png" width="32"/>
<a href="https://www.freebsd.org/"><img src="https://www.brueffer.io/project/freebsd/featured.png" width="32"/><a/>
<a/>

| OS            | Compilers     | Architectures  |
| ------------- |:-------------:| -----          |
| Linux         | GNU GCC, LLVM Clang | x86, amd64 |
| Windows       | MSVC, LLVM Clang for Windows | x86, amd64 |
| FreeBSD       | LLVM Clang    | x86, amd64        |

(Depending on the way the project evolves, FreeBSD support may be eliminated in favor of stability)

### Unsupported Platforms
| OS            | Compilers     | Architectures  |
| ------------- |:-------------:| -----          |
| MacOS         | N/A           | N/A            |
| iOS           | N/A           | N/A            |
| Android       | N/A           | N/A            |  

(If I can get a working M1 Mac sometime in the future I will work on porting the engine to MacOS and iOS)

### Build instructions / requirements

Warning: This repo is, BYOL, bring your own libraries

#### Compilation requirements

* CMake version 3.19 and above
* LLVM Clang / GNU GCC / Microsoft MSVC (any modern C/C++ compiler)
* libGLEW  (BYOL)
* libGLFW3 (BYOL)
* (Windows) Vulkan SDK / (Linux / BSD) Vulkan Libraries / Headers
* A SPIR-V compiler (Vulkan SDK in Windows, Package in Linux)

#### Compiling the code

1. Create the project files using CMake
    1. (Linux / FreeBSD) Run configure.sh
    2. (Windows) Use cmake/cmake-gui to create the project files
2. Compile the project
    1. (Make - Linux / FreeBSD) ```cd``` into the resulting build folder and run ```make```
    2. (VisualStudio - Windows) Open the resulting .vsproj and compile it 
3. Compile the shaders
    1. (Linux / FreeBSD) Run compile-shaders.sh
    2. (Windows) Provide your SPIR-V compiler with the shader source which is in ```projects/renderers/shaders``` and move the resulting binaries to the compiled executable directory under the  ```data/shaders``` folder

(The reasoning behind automating Linux / FreeBSD is that bash and other shells are more powerful and easy to automate than batch, I may look into adding powershell scripts but for now Windows is more DIY than the Linux / FreeBSD side of things)

#### Credits
Thanks <a href=forthebadge.com>forthebadge.com</a> for the badges!<br/>
