#!/bin/sh

if ! type -p cmake; then
   printf "Please install CMake using your distro's package manager\n\tDebian / Ubuntu: apt install cmake\n\tArch / Manjaro: pacman -S cmake\n\t..."
   exit 0
fi

GCC_FOUND=0
GCC_PATH=""

if GCC_PATH="$(type -p gcc)"; then
   GCC_FOUND=1
   printf "Found gcc!\n"
fi

CLANG_FOUND=0
CLANG_PATH=""

if CLANG_PATH="$(type -p clang)"; then
   CLANG_FOUND=1
   printf "Found clang!\n"
fi

if [[ $CLANG_FOUND == 0 ]] && [[ $GCC_FOUND == 0 ]]; then
   printf "Please install a supported C/C++ compiler using your distro's package manager, Clang and GCC are supported\n"
   printf "\tDebian / Ubuntu: apt install (gcc or clang)\n\tArch / Manjaro: pacman -S (gcc or clang)\n\t...\n"
   exit 0
fi

USED_COMPILER_C=""
USED_COMPILER_CPP=""

if [[ $CLANG_FOUND == 0 ]] && [[ $GCC_FOUND == 1 ]]; then
   USED_COMPILER_C="gcc"
   USED_COMPILER_CPP="g++"

   printf "Using gcc by default\n"
fi

if [[ $CLANG_FOUND == 1 ]] && [[ $GCC_FOUND == 0 ]]; then
   USED_COMPILER_C="clang"
   USED_COMPILER_CPP="clang++"

   printf "Using clang by default\n"
fi

if [[ $CLANG_FOUND == 1 ]] && [[ $GCC_FOUND == 1 ]]; then
   printf "Please choose your choose your compiler\n"
   printf "Clang or gcc?\n"

   if [[ $1 == "" ]]; then
      read COMPILER_CHOICE_TEMP
   else
      COMPILER_CHOICE_TEMP=$1
   fi

   if [[ $COMPILER_CHOICE_TEMP =~ [Cc]lang ]]; then
      USED_COMPILER_C="clang"
      USED_COMPILER_CPP="clang++"
      printf "Using clang\n"
   fi

   if [[ $COMPILER_CHOICE_TEMP =~ [Gg]cc ]]; then
      USED_COMPILER_C="gcc"
      USED_COMPILER_CPP="g++"
      printf "Using gcc\n"
   fi
fi

if [[ $USED_COMPILER_C == "" ]] || [[ $USED_COMPILER_CPP == "" ]]; then
   printf "You didn't choose a compiler!\n"
   exit 0
fi

BUILD_MODE_TYPE=""

printf "Please choose your build type\n"
printf "Release or Debug?\n"

if [[ $2 == "" ]]; then
   read BUILD_MODE_TYPE_TEMP
else
   BUILD_MODE_TYPE_TEMP=$2
fi

if [[ $BUILD_MODE_TYPE_TEMP =~ [Dd]ebug ]]; then
   BUILD_MODE_TYPE="Debug"
fi

if [[ $BUILD_MODE_TYPE_TEMP =~ [Rr]elease ]]; then
   BUILD_MODE_TYPE="Release"
fi

if [[ $BUILD_MODE_TYPE == "" ]]; then
   printf "No valid build type set, defaulting to Debug\n"
   BUILD_MODE_TYPE="Debug"
else
   printf "Building as $BUILD_MODE_TYPE\n"
fi

BUILD_PATH="cmake-build-$USED_COMPILER_C-$(echo $BUILD_MODE_TYPE | tr '[:upper:]' '[:lower:]')"

cmake -DCMAKE_BUILD_TYPE="$BUILD_MODE_TYPE" -DCMAKE_C_COMPILER="$USED_COMPILER_C" -DCMAKE_CXX_COMPILER="$USED_COMPILER_CPP" -S . -B $BUILD_PATH
ln -sf "$BUILD_PATH/compile_commands.json" "compile_commands.json"

printf "Generated symlink to compile_commands.json for vim autocomplete (coc)\n" 
printf "Active compiler is $USED_COMPILER_C/$USED_COMPILER_CPP\n"

printf "Generating .MANTA_CONFIG\n"

printf "CMAKE_PATH=${BUILD_PATH}\nBUILD_PATH=$(cat ".MANTA_CMAKE_DIR")" > .MANTA_CONFIG


