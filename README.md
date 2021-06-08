# Manta Engine

Thanks <a href=forthebadge.com>forthebadge.com</a> for these badges!<br/>
[![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-resentment.svg)](https://forthebadge.com) 

This is a small passion project that isn't intended to be ground breaking in any way. I am still learning better programming practices and have only been seriously using C/C++ for about a year now, any bugs and issues with this software are par for the course considering that it was written by a 16 year old...

This software was written using NeoVim with many customizations to make it adequate for C/C++ programming. Any formatting issues are the result of Vim.

<!-- I am aware this table is a mess, just let it be for now, it isn't hurting anyone -->

### Supported Platforms
| OS            | Compilers     | Architectures  |
| ------------- |:-------------:| -----          |
| Linux         | GNU GCC, LLVM Clang | x86, amd64 |
| Windows       | MSVC, LLVM Clang for Windows | x86, amd64 |
| FreeBSD       | LLVM Clang    | x86, amd64        |

(FreeBSD will potentially be zeroed from support)

### Unsupported Platforms
| OS            | Compilers     | Architectures  |
| ------------- |:-------------:| -----          |
| MacOS         | ENTIRELY UNSUPPORTED | ...         |

### Build instructions / requirements

Warning: This repo is, BYOL, bring your own libraries

#### Compilation requirements

* CMake version 3.19 and above
* LLVM Clang / GNU GCC / Microsoft MSVC (any modern C/C++ compiler)
* libGLEW  (BYOL)
* libGLFW3 (BYOL)
* (Windows) Vulkan SDK / (Linux / BSD) Vulkan Libraries / Headers

#### (Linux Only) GNU LibC is tested, Musl support is not guaranteed
#### For manual shader development you'll need a SPIR-V compiler

Using CMake, generate the project files and then use the resulting project files to compile the source, report any failed compilcation attempts to the issues page and I'll assist you with compilation!
