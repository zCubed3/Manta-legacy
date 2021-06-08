cmake_minimum_required(VERSION 3.19)
project(Manta)

set(CMAKE_CXX_STANDARD 17)

set(PROJECTS_DIR ${CMAKE_SOURCE_DIR}/projects)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}_${CMAKE_SYSTEM_PROCESSOR}/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# Tells CMake to generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Removes prefix from libraries since we don't need it
set(CMAKE_SHARED_LIBRARY_PREFIX "")


# Builds the "engine", which is just the loader that brings all the modules together
add_subdirectory(${PROJECTS_DIR}/engine)

# Comment out elements in this list to determine the render apis compiled into the engine
set(RENDER_APIS
   "COMPILE_GL" # Universal
   #"COMPILE_VK" # Universal
   
   #"COMPILE_DX11" # Windows only
   #"COMPILE_DX12" # Windows only
)

if(RENDER_APIS IN_LIST "COMPILE_GL")
   message("Compiling OpenGL backend.")
   add_subdirectory(${PROJECTS_DIR}/renderers/opengl)
endif()

if(RENDER_APIS IN_LIST "COMPILE_VK")
   message("Compiling Vulkan backend.")
   add_subdirectory(${PROJECTS_DIR}/renderers/vulkan)
endif()
