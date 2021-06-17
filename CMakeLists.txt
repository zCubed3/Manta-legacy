cmake_minimum_required(VERSION 3.19)
project(Manta)

set(CMAKE_CXX_STANDARD 17)

set(PROJECTS_DIR ${CMAKE_SOURCE_DIR}/projects)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}/")
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Tells CMake to generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Removes prefix from libraries since we don't need it
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(ENGINE_DIR ${PROJECTS_DIR}/engine)
set(DEPENDS_DIR ${CMAKE_SOURCE_DIR}/dependencies)

# Builds the "engine", which is just the loader that brings all the modules together
add_subdirectory(${ENGINE_DIR})

# Create info for build.sh to use
file(WRITE ".MANTA_CMAKE_DIR" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
